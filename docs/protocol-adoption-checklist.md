# Protocol Adoption Checklist

Use this checklist whenever UnityLink or UnityGPU specs evolve to keep UnityLobby and UnityXX clients in sync.

## UnityLobby protocol updates
- Update or add spec docs under `docs/protocol/` (UnityLink, UnityGPU Stream, TLVs, registries).
- If a frozen spec changes, add an entry to `docs/protocol/v0.2-errata.md` (or the relevant errata file) instead of rewriting history.
- Update or add registry entries in `docs/protocol/registries-*.md`.
- If norms change, update `docs/protocol/STYLE.md`.

## Test vectors and tooling
- Add or update byte-accurate frames in `test-vectors/frames/`.
- Regenerate binaries with `make vectors`.
- Update any tooling that encodes/decodes the affected frames in `tools/`.

## Cross-repo compatibility
- Update `docs/compatibility-matrix.md` with the new supported versions.
- Run `make compat-sync` to propagate the Unity64 table updates.

## UnityXX client coordination (Unity64 first)
- Identify required client capabilities and update the UnityLink capability bitset registry.
- Add implementation notes to `Unity64/docs/architecture.md` (protocol responsibility map).
- Update Unity64 firmware release notes when protocol support ships.

## Release and communication
- Add changelog notes in `CHANGELOG.md`.
- Update `README.md` quick links if new docs are introduced.
