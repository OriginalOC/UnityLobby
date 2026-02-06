# UnityLobby

UnityLobby is the reference server for the Unity ecosystem: a content-addressed app repository, catalog/manifest service, save storage, and multiplayer lobby/relay for UnityXX cartridges.

It implements **UnityLink** (MCU-first TCP protocol) and **UnityGPU Stream** (display-list rendering stream) to support discovery, installs, updates, cross-platform play, and spectating across classic platforms:
- Unity20 (VIC-20)
- Unity64 (Commodore 64)
- Unity99 (TI-99/4A)
- Unity8 (Atari 8-bit)
- Unity26 (Atari 2600)
- Unity52 (Atari 5200)
- Unity78 (Atari 7800)

## Status
- Protocol: **UnityLink v0.2 (spec frozen, implementation in progress)**
- Next: UnityLink v0.3 (install transactions, UnityIEC proxy/cache, observability)

## What’s in this repo
- [[docs/protocol/](docs/protocol/)](docs/protocol/) — UnityLink + UnityGPU specs, registries, TLVs, errata, and v0.3 handoff
- [[docs/architecture.md](docs/architecture.md)](docs/architecture.md) — system overview and client/server split
- [[docs/UnityXX.md](docs/UnityXX.md)](docs/UnityXX.md) — UnityXX family overview
- [[server/](server/)](server/) — UnityLobby server implementation (to be built out)
- [[test-vectors/](test-vectors/)](test-vectors/) — byte-accurate frames and hash vectors for cross-implementation compatibility
- [[tools/](tools/)](tools/) — helpers for encoding/decoding frames and generating test vectors

## Quick links
- [Architecture](docs/architecture.md)
- [UnityXX Overview](docs/UnityXX.md)
- [UnityLink v0.2](docs/protocol/unitylink-v0.2.md)
- [UnityGPU Stream v0](docs/protocol/unitygpu-stream-v0.md)
- [App Metadata TLVs v0](docs/protocol/app-metadata-tlvs-v0.md)
- [Registries (v0.2)](docs/protocol/registries-v0.2.md)
- [v0.2 Errata](docs/protocol/v0.2-errata.md)
- [v0.3 Handoff](docs/protocol/v0.3-handoff.md)
- [Cross-Repo Compatibility Matrix](docs/compatibility-matrix.md)
- [Protocol Adoption Checklist](docs/protocol-adoption-checklist.md)

## Docs sync
Use `make compat-sync` to update Unity64 compatibility tables from the matrix, and `make compat-check` to verify they are in sync.

## License
Apache-2.0. See [`LICENSE`](LICENSE) and [`NOTICE`](NOTICE)

## Contributing
This project is early, but we want it to be boring in the best way: stable specs, deterministic behavior, and test vectors that make every implementation match.

See [[CONTRIBUTING.md](CONTRIBUTING.md)](CONTRIBUTING.md)
