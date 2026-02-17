#!/usr/bin/env python3
import re
import sys
from pathlib import Path


def extract_contract_ids_from_doc(text: str):
    return sorted(set(re.findall(r"`([a-z0-9\-]+)`", text)))


def extract_contract_ids_from_tests(text: str):
    return sorted(set(re.findall(r"//\s*CONTRACT:\s*([a-z0-9\-]+)", text)))


def main() -> int:
    if len(sys.argv) != 3:
        print("Usage: check_coverage.py <contract_doc.md> <test_source.cpp>")
        return 2

    doc_path = Path(sys.argv[1])
    test_path = Path(sys.argv[2])

    doc_ids = extract_contract_ids_from_doc(doc_path.read_text(encoding="utf-8"))
    test_ids = extract_contract_ids_from_tests(test_path.read_text(encoding="utf-8"))

    tracked_ids = [
        cid for cid in doc_ids
        if cid in {
            "menu-open-on-hotkey",
            "registry-drives-config",
            "start-button-replacement",
            "skin-load-fallback",
            "multi-monitor-fallback",
            "menu-population",
            "wine-feature-degradation",
        }
    ]

    missing = [cid for cid in tracked_ids if cid not in test_ids]

    if missing:
        print("Coverage check failed. Missing contract tests for:")
        for contract_id in missing:
            print(f"  - {contract_id}")
        return 1

    print("Coverage check passed.")
    print("Contracts checked:", ", ".join(tracked_ids))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

# [CODEX] Last modified by: Codex
# [CODEX] Phase: 4
# [CODEX] Summary: Added contract coverage checker for behavior contract vs tests.
