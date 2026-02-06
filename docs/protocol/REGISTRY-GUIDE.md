# Registry Guide (UnityLobby)

Registries are the stability backbone for UnityLink/UnityGPU. This guide defines how to evolve IDs without breaking compatibility.

## Where registries live
- [registries-v0.2.md](registries-v0.2.md) is the authoritative registry document for v0.2
- New protocol versions add new registry docs (e.g., [registries-v0.3.md](registries-v0.3.md)) or extend with clearly labeled sections

## Rules (normative)
- IDs MUST NOT be reused for different meanings
- IDs MUST remain stable once published
- “Reserved” means not assigned yet; it may be assigned later
- “Deprecated” means still recognized but not recommended for new use

## When to add a registry entry
Add a registry entry whenever you introduce:
- a new message type
- a new enum value (entrypoint_type, role, compression, rng_id, etc.)
- a new TLV type
- a new GPU opcode or GPU stream version
- a new error detail code (v0.3+)

## How to propose changes
For PRs touching registries:
- Include motivation and compatibility impact
- Include caps/limits if the entry impacts sizing
- Include at least one test vector if the entry affects on-wire behavior

## Message type allocation policy
- Keep message type ranges grouped by feature area:
  - 0x0000–0x00FF: session/time
  - 0x0100–0x01FF: catalog/manifests
  - 0x0200–0x02FF: objects
  - 0x0300–0x03FF: saves
  - 0x0400–0x04FF: rooms/netplay/gpu
  - 0x0500–0x05FF: installs/transactions (v0.3 target)
  - 0x0600–0x06FF: cache/proxy (v0.3 target)
  - 0x0700–0x07FF: stats/observability (v0.3 target)

This keeps parsers and documentation predictable.

## TLV type allocation policy
- Use 0x1000–0x1FFF for App Metadata TLVs
- Use 0x2000–0x2FFF for Session TLVs (HELLO/WELCOME extensions)
- Use 0x3000–0x3FFF for Room TLVs
- Use 0xF000–0xFFFF for experimental/private TLVs (must not ship as stable)

## GPU opcode allocation policy
- 0x00–0x7F: standard opcodes
- 0x80–0xBF: experimental opcodes
- 0xC0–0xEF: platform-specific opcodes (only if negotiated later)
- 0xF0–0xFF: reserved/system (NOP/END_FRAME etc.)

## Deprecation
If an entry becomes obsolete:
- Mark it Deprecated in the registry
- Keep recognition behavior defined (ignore, treat as error, etc.)
- Remove only in a new major protocol version
