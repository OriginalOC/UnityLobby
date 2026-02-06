# Tools

This folder contains small utilities used by UnityLobby development and protocol test vectors.

- `hexframes_to_bin.py`: converts [`test-vectors/frames/*.hex`](../test-vectors/frames/) (with comments/whitespace) into raw `.bin` frames

Rule:
- Generated binaries (`*.bin`) are not intended to be committed. Use `make vectors` to regenerate locally.
