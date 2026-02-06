# Contributing to UnityLobby

Thanks for helping build UnityLobby and the UnityLink ecosystem.

This repo is both:
- a reference implementation (UnityLobby server)
- the canonical protocol documentation (UnityLink + UnityGPU + registries + test vectors)

The project goal is stability and determinism: MCU-friendly specs, tight registries, and test vectors so independent implementations match byte-for-byte.

## Ground rules
- Be precise. Prefer small, reviewable changes.
- Never introduce unbounded allocations in protocol parsing.
- Always add or update test vectors when changing any wire format.
- Prefer additive changes. Breaking changes require a clear migration story.

## Repo structure
- [docs/protocol/](docs/protocol/) — canonical specs and registries
- [server/](server/) — UnityLobby daemon implementation
- [test-vectors/](test-vectors/) — frames, sha256/crc32 vectors
- [tools/](tools/) — helpers to generate/verify vectors

## Version policy
### UnityLink v0.2
- Treated as **frozen**
- Corrections and clarifications go into [docs/protocol/v0.2-errata.md](docs/protocol/v0.2-errata.md)
- Do not rewrite [docs/protocol/unitylink-v0.2.md](docs/protocol/unitylink-v0.2.md) except for formatting or clearly non-semantic fixes

### UnityLink v0.3
- New work should go into new docs (e.g. [docs/protocol/unitylink-v0.3.md](docs/protocol/unitylink-v0.3.md))
- Keep proposals aligned with [docs/protocol/v0.3-handoff.md](docs/protocol/v0.3-handoff.md)
- Prefer capability-gated, additive features

## Specs: how to propose changes
1) Open an issue first for anything that affects wire format, registries, or backpressure rules
2) Include:
   - motivation (what breaks without this)
   - compatibility impact (v0.2 vs v0.3)
   - exact field layout (bytes + endianness)
   - max sizes / caps
   - failure modes and status codes
3) If it’s accepted:
   - update spec doc(s)
   - update registries
   - add test vectors demonstrating the change

## Errata workflow (v0.2)
- Add an entry to [docs/protocol/v0.2-errata.md](docs/protocol/v0.2-errata.md)
- Format:
  - Errata-XXXX: title, date
  - Affected sections
  - Old text / new text (or a concise description)
  - Rationale
- Errata should be minimal and factual

## Test vectors (required for protocol changes)
When a message layout changes or a new message is added:
- Add a hexdump under [test-vectors/frames/](test-vectors/frames/)
- Add any referenced object/hash data under [test-vectors/sha256/](test-vectors/sha256/) and [test-vectors/crc32/](test-vectors/crc32/)
- Keep vectors small and deterministic

## Code contributions (server/)
- Keep server components modular: session, catalog, manifest store, object store, saves, rooms
- Avoid language-specific assumptions in protocol docs
- Prefer clear logging and bounded memory usage

## Commit style
- Use short, descriptive commits
- Examples:
  - "docs: add v0.2 errata for ROOM join timeline"
  - "spec: add v0.3 install transaction draft"
  - "tools: add frame encoder for test vectors"

## Security
If you find a security issue:
- Do not open a public issue
- Follow [SECURITY.md](SECURITY.md)

## Code of Conduct
Be respectful. Assume good intent. Keep feedback actionable.
