# UnityLink v0.2

UnityLink is the MCU-first TCP protocol used by UnityLobby and UnityXX clients.

This document is the canonical v0.2 spec location for the project. The repo currently treats the following as authoritative references while the full text is being consolidated here:

- Registries: `registries-v0.2.md`
- App Metadata TLVs: `app-metadata-tlvs-v0.md`
- UnityGPU Stream: `unitygpu-stream-v0.md`
- Test vectors: `../../test-vectors/frames/`

## Status
- v0.2: frozen (wire format must remain stable)
- Corrections: go to `v0.2-errata.md`
- Next: v0.3 planning in `v0.3-handoff.md`

## Invariants
- Little-endian integers
- Length-prefixed frames (length excludes the 4-byte length field)
- Explicit caps for variable-length fields
- Content addressed immutable objects by SHA-256

## TODO
Paste the consolidated v0.2 spec text here (framing, messages, saves, rooms, GPU stream integration) while keeping v0.2 stable via errata.
