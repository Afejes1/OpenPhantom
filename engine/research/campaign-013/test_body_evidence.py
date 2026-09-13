"""Focused fail-closed checks using owned local reports; no game execution.

python engine/research/campaign-013/test_body_evidence.py --reference OWNED/WMAIN.EXE
"""
import argparse
from copy import deepcopy
from pathlib import Path
from unittest.mock import patch
import body_evidence as evidence
from formats import VerificationError


def run(reference):
    cases = [evidence.ENGINE / ("research/campaign-013-" + name) / "result.json"
             for name in ("music-resume", "sound-is-muted")]
    checks = 0
    for path in cases:
        output = evidence.extract(path, reference)
        assert output["status"] == "body-exact-layout-pending"
        assert output["full_function_exact"] is False
        assert output["body_size"] < output["full_size"]
        checks += 1
    result_path = cases[0]
    target_path = result_path.parent / "target.json"
    original_read = evidence.read
    result = original_read(result_path)
    docker_path = evidence.ROOT / result["docker_report"]
    target_key = target_path.relative_to(evidence.ENGINE).as_posix()
    try:
        evidence.extract(result_path, reference, native_report=evidence.ROOT / result["native_report"])
    except VerificationError:
        checks += 1
    else:
        raise AssertionError("one-sided fresh report override accepted")
    repeated = evidence.extract(result_path, reference,
                                evidence.ROOT / result["native_report"], docker_path)
    assert repeated["full_function_exact"] is False
    checks += 1
    mutations = [
        (result_path, lambda d: d["source_snapshot"].__setitem__(target_key, "0" * 64)),
        (result_path, lambda d: d.__setitem__("native_report_sha256", "0" * 64)),
        (docker_path, lambda d: d["fixture"].__setitem__("passed", False)),
        (docker_path, lambda d: d["artifacts"][0].__setitem__("object_sha256", "0" * 64)),
        (target_path, lambda d: d.__setitem__("body_size", d["body_size"] + 1)),
        (target_path, lambda d: d["bindings"].pop()),
    ]
    for selected, mutate in mutations:
        def changed_read(path):
            data = original_read(path)
            if path == selected:
                data = deepcopy(data)
                mutate(data)
            return data
        with patch.object(evidence, "read", side_effect=changed_read):
            try:
                evidence.extract(result_path, reference)
            except VerificationError:
                checks += 1
            else:
                raise AssertionError("altered evidence was accepted: " + str(selected))
    print("Body evidence: %d checks, 0 failures" % checks)
    return checks


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", required=True, type=Path)
    args = parser.parse_args()
    run(args.reference.resolve())
