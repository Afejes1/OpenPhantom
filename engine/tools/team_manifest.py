"""Import scheduling evidence without granting byte-matching acceptance."""
from collections import Counter
import hashlib
import re

from formats import require

REFERENCE_MD5 = "7c5af8428c19b17cca09ae3a49bd10ef"
REFERENCE_SIZE = 829952
REFERENCE_SHA256 = "02babee8d64bc0bf47451aaae9af630f4a028cf63f9996bda6df497caba34f11"
ROW = re.compile(r"(0x[0-9a-fA-F]{8})\s+(\(file not known yet\)|[A-Za-z0-9_/.-]+\.c)\s+(.+)")


def parse_manifest(data):
    text = data.decode("utf-8-sig")
    identity = re.search(r"WMAIN\.EXE \(([0-9,]+) bytes, md5 ([0-9a-f]{32})\)", text)
    require(identity is not None, "manifest reference identity missing")
    require(int(identity[1].replace(",", "")) == REFERENCE_SIZE and identity[2] == REFERENCE_MD5,
            "manifest targets a different retail binary")
    declared, rows, seen = {}, [], set()
    status = None
    for line_number, line in enumerate(text.splitlines(), 1):
        heading = re.fullmatch(r"(Done|Not done) \((\d+)\)", line)
        if heading:
            status = "done" if heading[1] == "Done" else "not_done"
            require(status not in declared, "duplicate manifest section")
            declared[status] = int(heading[2])
        if not line.startswith("0x"):
            continue
        match = ROW.fullmatch(line)
        require(match is not None and status is not None, "invalid manifest row")
        address = int(match[1], 16)
        require(0x401000 <= address < 0x499214, "address outside manifest application scope")
        require(address not in seen, "duplicate or conflicting manifest address")
        seen.add(address)
        module = None if match[2] == "(file not known yet)" else match[2]
        require(module is None or (not module.startswith("/") and ".." not in module.split("/")),
                "invalid reported module path")
        rows.append(dict(address=address, reported_file=module, reported_name=match[3],
                         reported_status=status, source_line=line_number))
    require(set(declared) == {"done", "not_done"}, "both manifest sections required")
    require(all(sum(r["reported_status"] == s for r in rows) == n for s, n in declared.items()),
            "manifest counts do not match rows")
    return dict(schema=1, reported_date="2026-09-12", source_name="Functions done or not.txt",
                source_sha256=hashlib.sha256(data).hexdigest(), reference_md5=REFERENCE_MD5,
                reference_size=REFERENCE_SIZE, reported_counts=declared, functions=rows)


def validate_manifest(doc):
    require(doc["schema"] == 1 and doc["reference_md5"] == REFERENCE_MD5 and
            doc["reference_size"] == REFERENCE_SIZE, "invalid team manifest identity")
    require(re.fullmatch(r"[0-9a-f]{64}", doc["source_sha256"]) is not None, "invalid attachment digest")
    seen = set()
    for row in doc["functions"]:
        address = row["address"]
        require(type(address) is int and 0x401000 <= address < 0x499214 and address not in seen,
                "invalid or duplicate team address")
        seen.add(address)
        require(row["reported_status"] in ("done", "not_done"), "invalid reported status")
        module = row["reported_file"]
        require(module is None or (isinstance(module, str) and re.fullmatch(r"[A-Za-z0-9_/.-]+\.c", module)
                                  and not module.startswith("/") and ".." not in module.split("/")),
                "invalid reported file")
        require(isinstance(row["reported_name"], str) and row["reported_name"].strip()
                and "\n" not in row["reported_name"], "invalid reported name")
    require(dict(Counter(r["reported_status"] for r in doc["functions"])) == doc["reported_counts"],
            "normalized manifest counts differ")
    return doc


def verified_by_address(target, history):
    current = {s["id"]: s for s in target["functions"]}
    if not history:
        return {}
    return {f["spec"]["address"]: current[f["id"]] for f in history[-1]["functions"]
            if current.get(f["id"]) == f["spec"] and not f["exceptions"]}


def render_team_progress(doc, order, target, history):
    validate_manifest(doc)
    require(target["size"] == doc["reference_size"] and target["sha256"] == REFERENCE_SHA256,
            "team and local target identity differ")
    verified = verified_by_address(target, history)
    by_address = {r["address"]: r for r in doc["functions"]}
    require(set(verified).issubset(by_address), "verified function absent from team manifest; reconcile explicitly")
    require(order["schema"] == 1, "unsupported work order")
    priorities = order["module_order"]
    require(priorities and len(set(priorities)) == len(priorities), "invalid module work order")
    modules = {r["reported_file"] for r in doc["functions"]}
    require(all(p in modules for p in priorities), "priority module absent from manifest")
    local_not_done = sum(by_address[a]["reported_status"] == "not_done" for a in verified)
    local_done = len(verified) - local_not_done
    counts = doc["reported_counts"]
    lines = ["# Team manifest and reconstruction work order", "",
             "Snapshot: " + doc["reported_date"] + ". Scheduling evidence supplied by the contributor; team code/build evidence is pending.", "",
             "Reported done: **%d**. Reported not done: **%d**." % (counts["done"], counts["not_done"]),
             "Our last recorded matches cover **%d** team-not-done entries and **%d** team-done entries." % (local_not_done, local_done),
             "Team-not-done entries without a local recorded match: **%d**." % (counts["not_done"] - local_not_done), "",
             "These are separate status columns, not a combined completion certificate. See [current build evidence](progress.md) for source freshness and byte-match scope.", "",
             "Research links include focused matches awaiting a batch checkpoint. Only recorded matches contribute to the counts above.", "",
             "Address plus the pinned retail binary identifies a function. Reported names (including aliases) and file assignments are preserved as supplied, not automatically applied to Ghidra or accepted source definitions.", "",
             "## Work order", "", "1. **%s**: current local focus." % priorities[0]]
    lines += ["%d. **%s**: queued after the active module." % (i + 2, p) for i, p in enumerate(priorities[1:])]
    lines += ["", "Other reported-not-done modules follow; reported-done functions are deferred pending the team's code. Existing overlapping calibrations remain regression checks.", "",
              "The manifest asks for file-level coordination. Relay the active file above to the team; no message or exclusive upstream reservation has been made by this tooling.", "",
              "## Active module", "", "| Original address | Team name | Team status | Local evidence |", "|---|---|---|---|"]
    pending = {int(r["address"], 0): r for r in order["pending_candidates"]}
    require(len(pending) == len(order["pending_candidates"]), "duplicate pending candidate")
    for address, item in pending.items():
        require(address in by_address and by_address[address]["reported_status"] == "not_done",
                "pending candidate must be reported not done")
        source = item["source"]
        require(re.fullmatch(r"[A-Za-z0-9_./-]+", source) and not source.startswith("/")
                and ".." not in source.split("/"), "invalid candidate source path")
    for address, row in by_address.items():
        if row["reported_file"] != priorities[0]:
            continue
        if address in verified:
            s = verified[address]; local = "Recorded match: [%s](../%s)" % (s["id"], s["source"])
        elif row["reported_status"] == "done":
            local = "Deferred; team reports done, local verification pending"
        elif address in pending:
            local = "Research candidate (not registered): [research](../%s)" % pending[address]["source"]
        else:
            local = "Queued; no local match recorded"
        lines.append("| 0x%08X | %s | %s | %s |" % (address, row["reported_name"].replace("|", "&#124;"), row["reported_status"], local))
    lines += ["", "## Remaining work by reported file", "",
              "| Reported file | Team not done | Local recorded matches within that list | Remaining |", "|---|---|---|---|"]
    for module in sorted(modules, key=lambda m: (priorities.index(m) if m in priorities else len(priorities), m or "~")):
        rows = [r for r in doc["functions"] if r["reported_file"] == module and r["reported_status"] == "not_done"]
        if rows:
            matched = sum(r["address"] in verified for r in rows)
            lines.append("| %s | %d | %d | %d |" % (module or "File not known yet", len(rows), matched, len(rows) - matched))
    lines += ["", "## Reconciliation when team source arrives", "",
              "Retain this dated snapshot. Import a new dated snapshot when status changes. Join by retail identity and address, compare module assignments and aliases, then compile the team's source with the recorded original toolchain. Only our existing strict verifier can grant local acceptance; an upstream done label never does.", "",
              "Attachment SHA-256: `%s`. Full normalized inventory: [JSON](../planning/team-manifest-20260912.json)." % doc["source_sha256"], ""]
    return "\n".join(lines)
