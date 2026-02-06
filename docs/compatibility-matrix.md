# Cross-Repo Compatibility Matrix

This document tracks compatibility between UnityLobby protocol versions and Unity64 firmware releases. It is the shared source of truth for cross-repo coordination.

## Protocol baseline (UnityLobby)
- UnityLink v0.2: spec frozen, implementation in progress.
- UnityLink v0.3: planned (install transactions, UnityIEC proxy/cache, observability).
- UnityGPU Stream v0: current stream version.

## Unity64 firmware releases (recent)
- 0.7 (2025/12/04)
- 0.6.8 (2025/07/25)
- 0.6.7 (2025/06/14)
- 0.6.6 (2025/02/15)
- 0.6.5 (2025/01/20)
- 0.6.4 (2025/01/03)
- 0.6.3_Mega65 (2024/12/04)
- 0.6.3 (2024/12/02)
- 0.6.2 (2024/10/06)
- 0.6.1 (2024/09/16)
- 0.6 (2024/07/10)

Source: `Unity64/bin/TeensyROM/FW_Release_History.md`

## Compatibility matrix

| Unity64 FW | Release date | UnityLink | UnityGPU Stream | Notes |
| --- | --- | --- | --- | --- |
| 0.7 | 2025/12/04 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.8 | 2025/07/25 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.7 | 2025/06/14 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.6 | 2025/02/15 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.5 | 2025/01/20 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.4 | 2025/01/03 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.3_Mega65 | 2024/12/04 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.3 | 2024/12/02 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.2 | 2024/10/06 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6.1 | 2024/09/16 | TBD | TBD | Protocol compatibility not documented yet. |
| 0.6 | 2024/07/10 | TBD | TBD | Protocol compatibility not documented yet. |

## How to update
- When Unity64 firmware adopts UnityLink or UnityGPU versions, replace `TBD` with the supported protocol versions.
- If a firmware release supports multiple protocol versions, list them as `v0.2, v0.3` and add details in Notes.
- Add new firmware releases at the top as they ship.
