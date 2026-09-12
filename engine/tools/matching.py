"""Exact code comparison with explicitly justified address operands.

Objects are never rewritten. Scores from other tools do not enter this gate.
"""
import hashlib
import json
from pathlib import Path
import struct

from formats import COFF, PE, VerificationError, require


MATCHES = {"raw-code-match", "relocation-adjusted-match"}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def canonical_hash(value):
    return digest(json.dumps(value, sort_keys=True, separators=(",", ":")).encode())


def load_target(path):
    target = json.loads(Path(path).read_text(encoding="utf-8"))
    require(target.get("schema") == 1 and target.get("target") == "WMAIN", "unsupported target")
    require(isinstance(target["sha256"], str) and len(target["sha256"]) == 64,
            "invalid target hash")
    require(target["size"] > 0 and target["image_base"] == 0x400000, "invalid target identity")
    require(bool(target["functions"]), "empty target function inventory")
    ids, symbols, intervals = set(), set(), []
    for spec in target["functions"]:
        fid = spec["id"]
        require(fid and all(c.isascii() and (c.isalnum() or c == "_") for c in fid), "invalid function id")
        require(fid not in ids and spec["symbol"] not in symbols, "duplicate function")
        ids.add(fid)
        symbols.add(spec["symbol"])
        require(0 < spec.get("body_size", spec["size"]) <= spec["size"], "invalid function body extent")
        require(spec["size"] > 0 and spec["address"] >= target["image_base"], "invalid function extent")
        start, end = spec["address"], spec["address"] + spec["size"]
        require(not any(start < b and a < end for a, b in intervals), "overlapping functions")
        intervals.append((start, end))
        source = Path(spec["source"])
        require(not source.is_absolute() and ".." not in source.parts and source.parts[0] == "src",
                "source must be within engine/src")
        require(spec["flags"] in (["/Od", "/MT"], ["/O2", "/MT"], ["/Ox", "/MT"]),
                "unapproved candidate compiler options")
        source_language(spec)
        validate_bindings(spec)
    return target



def source_language(spec):
    suffix = Path(spec["source"]).suffix
    require(suffix in (".c", ".cpp"), "unsupported source language extension")
    expected = "c++" if suffix == ".cpp" else "c"
    require(spec.get("language", expected) == expected, "source extension and language differ")
    return expected


def binding_kind(binding):
    return binding.get("kind", "dir32")


def constant_bytes(binding):
    key = next((k for k in ("float32", "float64") if k in binding), None)
    require(key is not None, "binding has no numeric constant")
    try:
        return struct.pack("<f" if key == "float32" else "<d", binding[key])
    except (struct.error, OverflowError, TypeError) as error:
        raise VerificationError("invalid numeric constant") from error


def expected_operand(spec, binding):
    address = binding["address"] + binding.get("addend", 0)
    require(0 <= address <= 0xffffffff, "binding target exceeds x86 address space")
    if binding_kind(binding) == "rel32-call":
        return (address - (spec["address"] + binding["offset"] + 4)) & 0xffffffff
    return address


def validate_bindings(spec):
    occupied = set()
    for binding in spec["bindings"]:
        offset = binding["offset"]
        require(type(offset) is int and 0 <= offset <= spec["size"] - 4, "binding outside function")
        region = set(range(offset, offset + 4))
        require(not occupied & region, "overlapping bindings")
        occupied.update(region)
        kind = binding_kind(binding)
        require(kind in ("dir32", "rel32-call", "dir32-internal"), "unsupported binding kind")
        require(sum(k in binding for k in ("symbol", "float32", "float64", "target_offset")) == 1, "ambiguous binding")
        require(type(binding["address"]) is int and 0 <= binding["address"] <= 0xffffffff,
                "invalid binding address")
        require(type(binding.get("addend", 0)) is int and 0 <= binding.get("addend", 0) <= 0xffffffff,
                "unsupported binding addend")
        if kind == "dir32-internal":
            local = binding.get("target_offset")
            require(type(local) is int and 0 <= local < spec["size"],
                    "internal destination outside function")
            require(binding["address"] == spec["address"] + local and
                    binding.get("addend", 0) == 0, "internal destination/addend differs")
        elif kind == "rel32-call":
            require("symbol" in binding and offset >= 1 and binding.get("addend", 0) == 0,
                    "relative calls require an external symbol and zero addend")
        elif "symbol" not in binding:
            constant_bytes(binding)
        expected_operand(spec, binding)


def verify_bound_operand(original, code, spec, binding):
    offset = binding["offset"]
    value = struct.unpack_from("<I", code, offset)[0]
    require(value == expected_operand(spec, binding), "original relocation target differs")
    address = spec["address"] + offset
    if binding_kind(binding) == "rel32-call":
        # The instruction boundary is reviewed in Ghidra and recorded in the
        # evidence. This is a bounded operand verifier, not an x86 disassembler.
        require(code[offset - 1] == 0xe8, "relative binding is not a near CALL")
        require(not any(address - 3 <= r < address + 4 for r in original.relocations),
                "relative call overlaps a PE base relocation")
        target = binding["address"]
        require(any(s.flags & 0x20 and original.image_base + s.address <= target <
                    original.image_base + s.address + s.size for s in original.sections),
                "relative call target is not backed by code")
        original.read_va(target, 1)
    else:
        require(address in original.relocations, "operand is not a PE relocation")
        if binding_kind(binding) == "dir32-internal":
            require(any(s.flags & 0x20 and original.image_base + s.address <= value <
                        original.image_base + s.address + s.size for s in original.sections),
                    "internal destination is not backed by code")
        elif "symbol" not in binding:
            constant = constant_bytes(binding)
            require(original.read_va(value, len(constant)) == constant,
                    "original constant differs")
    return value


def verify_reference(data, target):
    require(len(data) == target["size"] and digest(data) == target["sha256"],
            "reference identity mismatch; the documented retail WMAIN.EXE is required")
    pe = PE(data)
    require(pe.image_base == target["image_base"], "reference image base mismatch")
    for spec in target["functions"]:
        validate_bindings(spec)
        code = pe.read_va(spec["address"], spec["size"])
        absolute = {b["offset"] for b in spec["bindings"] if binding_kind(b) != "rel32-call"}
        relocs = {at - spec["address"] for at in pe.relocations
                  if spec["address"] - 3 <= at < spec["address"] + spec["size"]}
        require(relocs == absolute, "reference relocation inventory disagrees with " + spec["id"])
        for binding in spec["bindings"]:
            verify_bound_operand(pe, code, spec, binding)
    return pe


def compare_function(original, obj, spec):
    validate_bindings(spec)
    expected = original.read_va(spec["address"], spec["size"])
    actual, relocations = obj.function(spec["symbol"])
    result = {"id": spec["id"], "address": spec["address"], "size": spec["size"],
              "candidate_size": len(actual), "body_size": spec.get("body_size", spec["size"]), "status": "mismatch",
              "literal_function_match": False, "layout_verified": False,
              "adjusted_bytes": 0, "object_sha256": digest(obj.data)}
    if len(actual) != len(expected):
        result["reason"] = "complete function extent differs"
        return result
    bindings = {b["offset"]: b for b in spec["bindings"]}
    require(set(relocations) == set(bindings), "COFF relocation inventory differs from expected operands")
    adjusted = set()
    resolved = bytearray(actual)
    call_bytes = 0
    internal_bytes = 0
    function_symbol = next(s for s in obj.symbols.values() if s.name == spec["symbol"])
    for offset, binding in bindings.items():
        relocation = relocations[offset]
        symbol = relocation.symbol
        is_call = binding_kind(binding) == "rel32-call"
        require(relocation.kind == (20 if is_call else 6), "COFF relocation kind differs from binding")
        addend = struct.unpack_from("<I", actual, offset)[0]
        require(addend == binding.get("addend", 0), "relocation addend differs")
        original_value = verify_bound_operand(original, expected, spec, binding)
        if is_call:
            require(actual[offset - 1] == 0xe8, "candidate relative binding is not a near CALL")
            call_bytes += 4
        if binding_kind(binding) == "dir32-internal":
            require(symbol.section == function_symbol.section and symbol.storage == 6 and
                    symbol.kind == 0 and symbol.value == binding["target_offset"],
                    "internal relocation references the wrong local label")
            internal_bytes += 4
        elif "symbol" in binding:
            require(symbol.name == binding["symbol"] and symbol.section == 0 and symbol.value == 0
                    and symbol.storage == 2, "relocation references the wrong external symbol")
        else:
            constant = constant_bytes(binding)
            require(obj.symbol_bytes(symbol, addend, len(constant)) == constant, "candidate constant differs")
        # Resolve only the verified fixup in a comparison buffer. Never rewrite
        # an object or the original executable.
        struct.pack_into("<I", resolved, offset, original_value)
        adjusted.update(range(offset, offset + 4))
    differences = [i for i, (a, b) in enumerate(zip(expected, resolved)) if a != b]
    result.update(adjusted_bytes=len(adjusted), compared_bytes=len(actual) - len(adjusted),
                  dir32_bytes=len(adjusted) - call_bytes, rel32_call_bytes=call_bytes,
                  internal_dir32_bytes=internal_bytes,
                  different_bytes=len(differences), raw_bytes_equal=actual == expected,
                  relocated_bytes_equal=resolved == expected,
                  reference_span_sha256=digest(expected), resolved_span_sha256=digest(resolved))
    if differences:
        result.update(reason="non-relocation bytes differ", first_difference_offset=differences[0])
    else:
        result["status"] = "raw-code-match" if not adjusted else "relocation-adjusted-match"
    return result


def compare_batch(original, objects, target):
    results = []
    for spec in target["functions"]:
        try:
            require(spec["id"] in objects, "missing candidate object")
            results.append(compare_function(original, COFF(objects[spec["id"]]), spec))
        except (VerificationError, UnicodeError) as exc:
            results.append({"id": spec["id"], "address": spec["address"], "size": spec["size"],
                            "status": "unresolved", "reason": str(exc)})
    return {"schema": 1, "scope": "complete-object-code-sections",
            "target_sha256": target["sha256"], "target_spec_sha256": canonical_hash(target),
            "complete": bool(results) and all(r["status"] in MATCHES for r in results),
            "whole_executable_match": False, "functions": results}


def check_regression(current, baseline):
    require(current.get("schema") == baseline.get("schema") == 1, "invalid report schema")
    require(current.get("scope") == baseline.get("scope") == "complete-object-code-sections",
            "incompatible report scopes")
    require(current["target_sha256"] == baseline["target_sha256"], "reference changed")
    require(current["target_spec_sha256"] == baseline["target_spec_sha256"], "function/binding specification changed")
    require(current.get("complete") is True and baseline.get("complete") is True,
            "only complete successful runs may form regression baselines")
    now = {r["id"]: r for r in current["functions"]}
    before = {r["id"]: r for r in baseline["functions"]}
    require(len(now) == len(current["functions"]) and len(before) == len(baseline["functions"]),
            "duplicate regression records")
    require(now and now.keys() == before.keys(), "function inventory changed")
    for fid, old in before.items():
        new = now[fid]
        require(new["status"] in MATCHES and old["status"] in MATCHES, "function lost its match: " + fid)
        for field in ("address", "size", "candidate_size", "compared_bytes", "adjusted_bytes"):
            require(new[field] == old[field], "comparison extent changed: " + fid)
        require(new["different_bytes"] == old["different_bytes"] == 0, "mismatch in accepted report")
        require(new["status"] == old["status"], "matching category changed: " + fid)
        for report in (new, old):
            require(report["relocated_bytes_equal"] is True and
                    report["reference_span_sha256"] == report["resolved_span_sha256"],
                    "resolved byte equality is missing: " + fid)
        require(new["reference_span_sha256"] == old["reference_span_sha256"], "reference span changed: " + fid)


def compare_whole(original, candidate):
    same = original == candidate
    first = next((i for i, pair in enumerate(zip(original, candidate)) if pair[0] != pair[1]), None)
    if first is None and not same:
        first = min(len(original), len(candidate))
    return {"schema": 1, "scope": "entire-file-no-exclusions", "whole_executable_match": same,
            "original_size": len(original), "candidate_size": len(candidate),
            "original_sha256": digest(original), "candidate_sha256": digest(candidate),
            "first_difference_offset": first}
