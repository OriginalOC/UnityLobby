#!/usr/bin/env python3
"""Sync Unity64 protocol compatibility tables from UnityLobby matrix."""
from __future__ import annotations

from pathlib import Path
import sys


def read_matrix_rows(matrix_path: Path) -> list[list[str]]:
    lines = matrix_path.read_text().splitlines()
    table_start = None
    for i, line in enumerate(lines):
        if line.strip().startswith("|") and "Unity64 FW" in line:
            table_start = i
            break
    if table_start is None:
        raise ValueError("compatibility matrix table not found")

    rows = []
    for line in lines[table_start + 2 :]:
        if not line.strip().startswith("|"):
            break
        parts = [p.strip() for p in line.strip().strip("|").split("|")]
        if len(parts) < 5:
            continue
        rows.append(parts)

    if not rows:
        raise ValueError("compatibility matrix table has no rows")
    return rows


def build_unity64_table(rows: list[list[str]]) -> str:
    header = "| Unity64 FW | UnityLink | UnityGPU Stream | Notes |"
    sep = "| --- | --- | --- | --- |"
    out = [header, sep]
    for fw, _date, link, gpu, notes in rows:
        out.append(f"| {fw} | {link} | {gpu} | {notes} |")
    return "\n".join(out)


def replace_section(path: Path, table: str) -> bool:
    text = path.read_text()
    heading = "## Protocol Compatibility (Merged)"
    lines = text.splitlines()

    try:
        start = next(i for i, line in enumerate(lines) if line.strip() == heading)
    except StopIteration:
        raise ValueError(f"missing compatibility heading in {path}")

    # Find the next section header or end of file.
    end = len(lines)
    for i in range(start + 1, len(lines)):
        if lines[i].startswith("## "):
            end = i
            break

    new_block = [
        heading,
        "",
        "This table is a quick view only. The canonical compatibility source is `UnityLobby/docs/compatibility-matrix.md`.",
        "",
    ] + table.splitlines()

    new_lines = lines[:start] + new_block + lines[end:]
    new_text = "\n".join(new_lines) + ("\n" if text.endswith("\n") else "")
    if new_text != text:
        path.write_text(new_text)
        return True
    return False


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    workspace = root.parent

    matrix = root / "docs" / "compatibility-matrix.md"
    if not matrix.exists():
        raise SystemExit(f"missing {matrix}")

    rows = read_matrix_rows(matrix)
    table = build_unity64_table(rows)

    targets = [
        workspace / "Unity64" / "README.md",
        workspace / "Unity64" / "Source" / "u64-teensy" / "README.md",
    ]

    if len(sys.argv) > 1 and sys.argv[1] in {"--check", "-c"}:
        mismatches = []
        for target in targets:
            text = target.read_text()
            start = text.find("## Protocol Compatibility (Merged)")
            if start == -1:
                mismatches.append(str(target))
                continue
            current = text[start:]
            expected = "## Protocol Compatibility (Merged)\\n\\n" + (
                "This table is a quick view only. The canonical compatibility source is `UnityLobby/docs/compatibility-matrix.md`.\\n\\n"
                + table
                + "\\n"
            )
            if not current.startswith(expected):
                mismatches.append(str(target))
        if mismatches:
            print("Compatibility tables are out of sync:")
            for path in mismatches:
                print(f"- {path}")
            return 1
        print("Compatibility tables are in sync.")
        return 0
    changed = False
    for target in targets:
        if replace_section(target, table):
            changed = True

    if changed:
        print("Updated Unity64 compatibility tables.")
    else:
        print("No changes needed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
