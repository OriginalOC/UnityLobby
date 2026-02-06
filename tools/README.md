# Tools

This folder contains small utilities used by UnityLobby development and protocol test vectors.

- `hexframes_to_bin.py`: converts [`test-vectors/frames/*.hex`](../test-vectors/frames/) (with comments/whitespace) into raw `.bin` frames
- `sync_compat_tables.py`: syncs Unity64 compatibility tables from `docs/compatibility-matrix.md`

Rule:
- Generated binaries (`*.bin`) are not intended to be committed. Use `make vectors` to regenerate locally.
- Use `make compat-sync` after updating the compatibility matrix to propagate tables into Unity64 READMEs.
- Use `make compat-check` to verify the tables are in sync without modifying files.
