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
        occupied = set()
        for binding in spec["bindings"]:
            offset = binding["offset"]
            require(0 <= offset <= spec["size"] - 4, "binding outside function")
            region = set(range(offset, offset + 4))
            require(not occupied & region, "overlapping bindings")
            occupied.update(region)
            require(("symbol" in binding) != ("float32" in binding), "ambiguous binding")
            require(0 <= binding["address"] <= 0xffffffff, "invalid binding address")
    return target


def verify_reference(data, target):
    require(len(data) == target["size"] and digest(data) == target["sha256"],
            "reference identity mismatch; the documented retail WMAIN.EXE is required")
    pe = PE(data)
    require(pe.image_base == target["image_base"], "reference image base mismatch")
    # Validate every operand before compiling or opening candidate artifacts.
    for spec in target["functions"]:
        code = pe.read_va(spec["address"], spec["size"])
        bindings = {b["offset"]: b for b in spec["bindings"]}
        relocs = {at - spec["address"] for at in pe.relocations
                  if spec["address"] - 3 <= at < spec["address"] + spec["size"]}
        require(relocs == set(bindings), "reference relocation inventory disagrees with " + spec["id"])
        for offset, binding in bindings.items():
            address = struct.unpack_from("<I", code, offset)[0]
            require(address == binding["address"] + binding.get("addend", 0),
                    "reference operand disagrees with " + spec["id"])
            if "float32" in binding:
                require(pe.read_va(address, 4) == struct.pack("<f", binding["float32"]),
                        "reference constant disagrees with binding")
    return pe


def compare_function(original, obj, spec):
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
    for offset, binding in bindings.items():
        symbol = relocations[offset]
        addend = struct.unpack_from("<I", actual, offset)[0]
        require(addend == binding.get("addend", 0), "relocation addend differs")
        original_value = struct.unpack_from("<I", expected, offset)[0]
        require(original_value == binding["address"] + addend, "original relocation target differs")
        require(spec["address"] + offset in original.relocations, "operand is not a PE relocation")
        if "symbol" in binding:
            require(symbol.name == binding["symbol"] and symbol.section == 0 and symbol.value == 0,
                    "relocation references the wrong external symbol")
        else:
            constant = struct.pack("<f", binding["float32"])
            require(obj.symbol_bytes(symbol, addend, 4) == constant, "candidate constant differs")
            require(original.read_va(original_value, 4) == constant, "original constant differs")
        # Resolve only this fully verified COFF fixup in a comparison buffer.
        # Neither the object file nor the original image is rewritten.
        struct.pack_into("<I", resolved, offset, binding["address"] + addend)
        adjusted.update(range(offset, offset + 4))
    differences = [i for i, (a, b) in enumerate(zip(expected, resolved)) if a != b]
    result.update(adjusted_bytes=len(adjusted), compared_bytes=len(actual) - len(adjusted),
                  different_bytes=len(differences), raw_bytes_equal=actual == expected,
                  relocated_bytes_equal=resolved == expected,
                  reference_span_sha256=digest(expected), resolved_span_sha256=digest(resolved))
    if differences:
        # No original bytes in reports; detailed disassembly stays in Ghidra/reccmp locally.
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
