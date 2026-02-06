# UnityXX Overview

UnityXX is the family of client cartridges that connect to UnityLobby over UnityLink. Each UnityXX device pairs platform-accurate runtime execution with a shared network protocol so apps can be distributed, verified, and run consistently across classic hardware.

## Members
- Unity20 (VIC-20)
- Unity64 (Commodore 64)
- Unity99 (TI-99/4A)
- Unity8 (Atari 8-bit)
- Unity26 (Atari 2600)
- Unity52 (Atari 5200)
- Unity78 (Atari 7800)

## Shared Responsibilities
- UnityLink session + framing
- Catalog discovery and manifest fetch
- Object streaming with SHA-256 verification
- Optional saves (commit-based)
- Optional rooms: lockstep input relay (Mode B) and GPU stream receive (Mode A)

## Split Of Responsibilities
- UnityXX clients: local UI, loader behavior, platform entrypoints, and runtime execution
- UnityLobby: catalogs, manifests, object storage, save persistence, and room relay
