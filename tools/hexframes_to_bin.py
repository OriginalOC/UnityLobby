#!/usr/bin/env python3
"""
hexframes_to_bin.py

Convert UnityLobby test-vector .hex files into raw .bin files.

Input format:
- May include lines starting with '#'(comments)
- May include whitespace anywhere
- Remaining content must be hex pairs (e.g. '4c000000 0002 ...')

Usage:
  tools/hexframes_to_bin.py path/to/file.hex
  tools/hexframes_to_bin.py path/to/file.hex -o path/to/file.bin
  tools/hexframes_to_bin.py test-vectors/frames --all
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from pathlib import Path

HEX_RE = re.compile(r"^[0-9a-fA-F]+$")


def hexfile_to_bytes(hex_path: Path) -> bytes:
    chunks: list[str] = []
    with hex_path.open("r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            # Strip inline comments starting with '#'
            if "#" in line:
                line = line.split("#", 1)[0].strip()
            if line:
                chunks.append(line)

    joined = "".join(chunks)
    joined = re.sub(r"\s+", "", joined)

    if len(joined) == 0:
        raise ValueError(f"{hex_path}: no hex data found")

    if len(joined) % 2 != 0:
        raise ValueError(f"{hex_path}: hex string has odd length ({len(joined)})")

    if not HEX_RE.match(joined):
        raise ValueError(f"{hex_path}: contains non-hex characters after stripping comments/whitespace")

    return bytes.fromhex(joined)


def write_bin(data: bytes, out_path: Path) -> None:
    out_path.parent.mkdir(parents=True, exist_ok=True)
    tmp_path = out_path.with_suffix(out_path.suffix + ".tmp")
    with tmp_path.open("wb") as f:
        f.write(data)
    os.replace(tmp_path, out_path)


def default_out_path(hex_path: Path) -> Path:
    return hex_path.with_suffix(".bin")


def convert_one(hex_path: Path, out_path: Path | None) -> Path:
    data = hexfile_to_bytes(hex_path)
    out = out_path if out_path else default_out_path(hex_path)
    write_bin(data, out)
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description="Convert UnityLobby .hex frame files to .bin")
    ap.add_argument("path", help="A .hex file OR a directory (with --all)")
    ap.add_argument("-o", "--out", help="Output .bin path (file mode only)")
    ap.add_argument("--all", action="store_true", help="If path is a directory, convert all *.hex under it")
    args = ap.parse_args()

    p = Path(args.path)

    try:
        if args.all:
            if not p.is_dir():
                print("ERROR: --all requires path to be a directory", file=sys.stderr)
                return 2
            hex_files = sorted(p.rglob("*.hex"))
            if not hex_files:
                print(f"ERROR: no .hex files found under {p}", file=sys.stderr)
                return 2
            for hp in hex_files:
                outp = convert_one(hp, None)
                print(f"{hp} -> {outp} ({outp.stat().st_size} bytes)")
            return 0

        # single file mode
        if p.is_dir():
            print("ERROR: path is a directory. Use --all to convert all .hex files.", file=sys.stderr)
            return 2
        if not p.exists():
            print(f"ERROR: not found: {p}", file=sys.stderr)
            return 2
        out = Path(args.out) if args.out else None
        outp = convert_one(p, out)
        print(f"{p} -> {outp} ({outp.stat().st_size} bytes)")
        return 0

    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
