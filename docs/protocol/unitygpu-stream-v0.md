# UnityGPU Stream v0

UnityGPU Stream v0 defines a compact display-list command buffer for rendering “draw intent” across UnityXX platforms.

Authoritative references:
- Opcode registry: [registries-v0.2.md](registries-v0.2.md) (UnityGPU Stream v0 Opcodes)
- Mode A transport: UnityLink room GPU frames (ROOM_GPU_FRAME)
- Mode B usage: local-only display list format shared across platforms

## Command format
Each command is:
- u8 opcode
- u8 len
- bytes[len] payload

Unknown opcodes MUST be skipped via len. End of frame is indicated by END_FRAME.

## Status
Stable for v0.x. Changes require a new UnityGPU stream version and registry updates.
