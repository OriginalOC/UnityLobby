# UnityLobby Server

UnityLobby is the reference server for the Unity ecosystem. It serves:
- Catalog + manifests (UnityLink)
- Content-addressed immutable objects (SHA-256)
- Saves storage (commit-based)
- Rooms for netplay relay, spectate, and GPU display list broadcast (Mode A)

This folder is the server implementation home. The protocol specs live in `docs/protocol/`.

## Goals
- MCU-first protocol parsing: bounded allocations, strict limits, predictable behavior
- Deterministic storage model: content addressed objects, replayable indexes
- UnitySTEAM direction: install transactions, caching proxy (UnityIEC), observability

## Run model (high level)
- One TCP listener on the configured port (default 6464)
- One connection per session
- Server enforces protocol caps, auth policy, and backpressure rules

## Configuration
Copy the example config and edit:
- `server/config/unitylobby.example.toml` -> your runtime config path, e.g. `server/config/unitylobby.toml`

Key settings:
- `net.listen_addr`, `net.port`
- `auth.mode` and `auth.tokens`
- `limits.*` for frame and payload caps
- `storage.root` for repo layout
- `rooms.*` for player/spectator limits and policies

## Storage layout
UnityLobby stores content in an on-disk repo rooted at `storage.root`. See `repo/README.md` for the recommended layout.

## Development notes
- Protocol docs are canonical. Implementations should match test vectors byte-for-byte.
- Do not relax limits in code without updating docs and test vectors.
- Prefer additive protocol changes and registry updates over breaking changes.
