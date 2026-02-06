# UnityLobby Architecture

UnityLobby is the reference server for the Unity ecosystem. It is designed to serve many classic platforms through a single MCU-first protocol (UnityLink), with a long-term goal of a UnitySTEAM-style library: discovery, installs, updates, saves, multiplayer, and spectating.

UnityLobby is the server-side hub. UnityXX cartridges (Unity20/64/99/8/26/52/78) are the client-side runtime that connects to it. See `docs/UnityXX.md` for a shared overview of the UnityXX clients.

For Unity64-specific details, see `docs/architecture.md` in the Unity64 repo.

## Components

### UnityXX clients (carts)
UnityXX refers to platform cartridges (Unity20/64/99/8/26/52/78). Each client implements:
- UnityLink session + framing
- Catalog + manifest fetch
- Object streaming with SHA-256 verification
- Optional saves (commit-based)
- Optional rooms: lockstep input relay and or GPU stream receive
- Local UI and loader behavior for that platform
- Mailbox bridge to a host computer (if present)
- Optional local caching of objects and metadata
- Platform-specific app launch (PRG/CRT/etc.)

Client capabilities vary by platform. UnityLink uses a capabilities bitset so UnityLobby can tailor responses.

### UnityLobby (server)
UnityLobby provides:
- Catalog listing (apps available)
- Manifests tailored to client platform/capabilities
- Content-addressed object store (immutable objects by sha256)
- Save storage (begin/data/commit)
- Rooms for netplay relay, spectate, and GPU display list broadcast

UnityLobby is intended to run on Linux and scale to multiple clients concurrently.

### UnityIEC (future proxy/cache)
UnityIEC is a planned proxy/cache device on the local network. It aims to:
- Cache objects (immutable, content addressed)
- Serve objects to UnityXX clients with lower latency
- Upstream to UnityLobby for misses
- Optionally prefetch popular content

UnityIEC should be able to speak UnityLink to clients and use UnityLink upstream to UnityLobby.

## Data model

### Objects
- Immutable content addressed blobs
- Addressed by sha256 (object_id)
- Verified by clients after download
- Cached safely anywhere (UnityIEC, clients) because immutability removes coherence complexity

### Apps and manifests
- Catalog lists apps (app_id + title + platform mask + latest version)
- Manifests list the objects required for a specific app version/platform
- Manifests are tailored so referenced compression formats are supported by the client

### Saves
- Keyed by (user_id, app_id, slot)
- Uploaded as begin/data/commit
- Visible only after commit succeeds
- Intended to support UnitySTEAM-style profiles and cloud saves later

### Rooms (multiplayer/spectate)
Rooms support:
- Mode B: lockstep input relay (cross-platform netplay)
- Mode A: GPU display list broadcast (remote render + spectators)

UnityLobby is a relay for inputs and GPU frames. It does not run the game. In lockstep, the simulation runs on the clients.

## Key design principles

### MCU-first protocol
- Fixed header framing, little-endian primitives
- Explicit caps on all lengths
- Additive evolution via TLV blocks and registries

### Determinism and verification
- SHA-256 verification for objects
- Optional CRC32 for early detection
- Lockstep netplay uses deterministic tick scheduling and optional state hashing

### UnitySTEAM direction
UnityLobby is intended to grow into:
- Install/update transactions (v0.3)
- UnityIEC proxy/cache mode (v0.3)
- Observability (stats, structured errors)
- Eventually richer identity/auth, presence, and library features

## Typical flows

### App discovery and install (v0.2)
1) Client connects, HELLO/WELCOME (platform id, capabilities, device name, optional auth token)
2) Client requests CATALOG_LIST
3) Client requests APP_MANIFEST_GET
4) Client streams required objects via OBJECT_GET
5) Client verifies sha256 and installs/loads locally

### Lockstep multiplayer (Mode B, v0.2)
1) Client creates or joins a room
2) Players exchange inputs via ROOM_INPUT relayed by UnityLobby
3) Each client runs deterministic simulation per tick
4) Optional ROOM_STATE_HASH messages detect desync

### Spectating / remote render (Mode A, v0.2)
1) Host creates a Mode A room
2) Host sends ROOM_GPU_FRAME (UnityGPU display lists) each tick/frame
3) UnityLobby broadcasts to players and spectators
4) Slow clients receive newest complete frames (drop policy)

## What changes in v0.3
v0.3 work should be additive and focus on:
- Install transactions (atomic, resumable)
- UnityIEC cache sync (object presence, prefetch)
- Stats and structured error detail codes
- Minimal auth evolution hooks

See [docs/protocol/v0.3-handoff.md](docs/protocol/v0.3-handoff.md)
