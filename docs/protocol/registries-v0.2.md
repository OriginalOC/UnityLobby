# Registries (UnityLink v0.2)

This document is the authoritative registry for all stable IDs used by UnityLink v0.2 and UnityGPU Stream v0.

Rules:
- IDs MUST NOT be reused for different meanings
- “Reserved” IDs may be assigned later
- “Deprecated” IDs must remain recognized until a major version bump

---

## R1. Protocol Versions

| Name | Value | Notes |
|---|---:|---|
| UnityLink v0.2 proto_version | 0x00000002 | Frame.reserved = (major<<16) \| minor, major=0 minor=2 |

---

## R2. Frame Flags (u16)

| Flag | Value | Meaning |
|---|---:|---|
| FLAG_RESPONSE | 0x0001 | Set on responses |
| FLAG_MORE | 0x0002 | More frames follow for this stream |
| Reserved | 0xFFFC | MUST be 0 in v0.2 |

---

## R3. Status Codes (u32)

| Status | Value | Meaning |
|---|---:|---|
| OK | 0 | Success |
| ERR_BAD_REQUEST | 1 | Malformed payload, invalid lengths, invalid fields |
| ERR_UNSUPPORTED | 2 | Feature/type not supported |
| ERR_NOT_FOUND | 3 | Missing object/app/slot/room |
| ERR_UNAUTHORIZED | 4 | Auth required or token invalid |
| ERR_BUSY | 5 | Too many inflight ops, throttling |
| ERR_INTERNAL | 6 | Server internal error |
| ERR_BAD_STATE | 7 | Wrong order/state (e.g., pre-WELCOME request) |
| ERR_RANGE | 8 | Offset/length out of range, caps exceeded |
| ERR_CHECKSUM | 9 | CRC32 mismatch or integrity failure (where applicable) |

---

## R4. Platform IDs (u32)

| Platform | platform_id |
|---|---:|
| Unity20 (VIC-20) | 20 |
| Unity64 (C64) | 64 |
| Unity99 (TI-99/4A) | 99 |
| Unity8 (Atari 8-bit) | 8 |
| Unity26 (Atari 2600) | 26 |
| Unity52 (Atari 5200) | 52 |
| Unity78 (Atari 7800) | 78 |

---

## R5. Platform Mask Index Table (u32 bit positions)

Platform mask uses fixed indices, not platform_id directly

| Index | platform_id |
|---:|---:|
| 0 | 20 |
| 1 | 64 |
| 2 | 99 |
| 3 | 8 |
| 4 | 26 |
| 5 | 52 |
| 6 | 78 |

Bit rule:
- mask includes platform_id P if `(mask & (1 << index(P))) != 0`

---

## R6. Capabilities (u64 bits)

### Base capabilities
| Bit | Name | Meaning |
|---:|---|---|
| 0 | CAP_STREAM | Can receive streamed chunks |
| 1 | CAP_RESUME | Supports offsets/resume |
| 2 | CAP_COMPRESS_RX | Can decompress object payload bytes |
| 3 | CAP_COMPRESS_TX | Can send compressed object payload bytes (unused by default) |
| 4 | CAP_SAVES | Supports save slots |
| 5 | CAP_TIME | Wants time sync |
| 6 | CAP_CACHE | Can cache objects (UnityIEC likely sets) |
| 7 | CAP_MAILBOX_BRIDGE | Can bridge host mailbox calls to UnityLink |

### v0.2 capabilities
| Bit | Name | Meaning |
|---:|---|---|
| 8 | CAP_NETPLAY | Supports rooms + netplay relay |
| 9 | CAP_SPECTATE | Supports spectator role |
| 10 | CAP_GPU_STREAM_RX | Can receive UnityGPU display lists |
| 11 | CAP_GPU_STREAM_TX | Can send UnityGPU display lists (producer) |
| 12 | CAP_STATE_HASH | Supports state hash messages |

Bits 13..63 reserved

---

## R7. Message Types (u16)

### Session and Time (0x0000–0x00FF)
| Type | ID | Direction | Name |
|---|---:|---|---|
| 0x0001 | 1 | C→S | HELLO_REQ |
| 0x0002 | 2 | S→C | WELCOME_RESP |
| 0x0003 | 3 | C→S | PING_REQ |
| 0x0004 | 4 | S→C | PONG_RESP |
| 0x0005 | 5 | C→S | TIME_GET_REQ |
| 0x0006 | 6 | S→C | TIME_GET_RESP |

### Catalog (0x0100–0x01FF)
| Type | ID | Direction | Name |
|---|---:|---|---|
| 0x0100 | 256 | C→S | CATALOG_LIST_REQ |
| 0x0101 | 257 | S→C | CATALOG_LIST_RESP |
| 0x0110 | 272 | C→S | APP_MANIFEST_GET_REQ |
| 0x0111 | 273 | S→C | APP_MANIFEST_GET_RESP |

### Objects (0x0200–0x02FF)
| Type | ID | Direction | Name |
|---|---:|---|---|
| 0x0200 | 512 | C→S | OBJECT_GET_REQ |
| 0x0201 | 513 | S→C | OBJECT_DATA_RESP |
| 0x0202 | 514 | S→C | OBJECT_END_RESP |

### Saves (0x0300–0x03FF)
| Type | ID | Direction | Name |
|---|---:|---|---|
| 0x0300 | 768 | C→S | SAVE_PUT_BEGIN_REQ |
| 0x0301 | 769 | C→S | SAVE_PUT_DATA_REQ |
| 0x0302 | 770 | C→S | SAVE_PUT_COMMIT_REQ |
| 0x0303 | 771 | S→C | SAVE_PUT_RESP |
| 0x0310 | 784 | C→S | SAVE_GET_REQ |
| 0x0311 | 785 | S→C | SAVE_DATA_RESP |
| 0x0312 | 786 | S→C | SAVE_END_RESP |
| 0x0320 | 800 | C→S | SAVE_LIST_REQ |
| 0x0321 | 801 | S→C | SAVE_LIST_RESP |

### Rooms / Netplay / GPU (0x0400–0x04FF)
| Type | ID | Direction | Name |
|---|---:|---|---|
| 0x0400 | 1024 | C→S | ROOM_LIST_REQ |
| 0x0401 | 1025 | S→C | ROOM_LIST_RESP |
| 0x0410 | 1040 | C→S | ROOM_CREATE_REQ |
| 0x0411 | 1041 | S→C | ROOM_CREATE_RESP |
| 0x0412 | 1042 | C→S | ROOM_JOIN_REQ |
| 0x0413 | 1043 | S→C | ROOM_JOIN_RESP |
| 0x0414 | 1044 | C→S | ROOM_LEAVE_REQ |
| 0x0415 | 1045 | S→C (evt) | ROOM_EVENT_EVT (request_id=0) |
| 0x0420 | 1056 | C→S | ROOM_INPUT_REQ |
| 0x0421 | 1057 | S→C (evt) | ROOM_INPUT_EVT (request_id=0) |
| 0x0430 | 1072 | C→S | ROOM_GPU_FRAME_REQ |
| 0x0431 | 1073 | S→C (evt) | ROOM_GPU_FRAME_EVT (request_id=0) |
| 0x0440 | 1088 | C→S | ROOM_STATE_HASH_REQ |
| 0x0441 | 1089 | S→C (evt) | ROOM_STATE_HASH_EVT (request_id=0) |

Reserved for v0.3:
- 0x0500–0x05FF installs/transactions
- 0x0600–0x06FF cache/proxy
- 0x0700–0x07FF stats/observability

---

## R8. Entrypoint Types (u32)

| Value | Name | Notes |
|---:|---|---|
| 0 | PRG | Commodore PRG |
| 1 | CRT | Commodore cartridge |
| 2 | ROM | Generic ROM blob |
| 3 | ATR | Atari 8-bit disk image |
| 4 | DSK | Disk image (generic; TI-99/4A etc.) |
| 5.. | Reserved | Extend in future registry version |

---

## R9. Object Roles (u32)

| Value | Name | Meaning |
|---:|---|---|
| 0 | ROLE_CODE | Executable/code |
| 1 | ROLE_ASSET | Art/audio/assets |
| 2 | ROLE_META | Metadata |
| 3 | ROLE_COVER | Cover image |
| 4 | ROLE_DOC | Documentation |
| 5.. | Reserved | |

---

## R10. Compression IDs (u32)

| Value | Name | Meaning |
|---:|---|---|
| 0 | NONE | No compression |
| 1 | LZ4 | Recommended for MCU |
| 2 | ZSTD | Optional |
| 3.. | Reserved | |

---

## R11. Room Roles (u8)

| Value | Name |
|---:|---|
| 0 | ROLE_PLAYER |
| 1 | ROLE_SPECTATOR |
| 2.. | Reserved |

---

## R12. Room Flags (u32)

| Bit | Name | Meaning |
|---:|---|---|
| 0 | ROOMF_PUBLIC | Anyone can join |
| 1 | ROOMF_PRIVATE | Join requires token |
| 2 | ROOMF_MODE_A_GPU | GPU display list broadcast mode |
| 3 | ROOMF_MODE_B_LOCKSTEP | Lockstep input relay mode |
| 4 | ROOMF_RECORD_INPUTS | Reserved (record/replay) |
| 5..31 | Reserved | |

---

## R13. Room Event Types (u32)

| Value | Name | Meaning |
|---:|---|---|
| 1 | JOINED | A client joined |
| 2 | LEFT | A client left |
| 3 | ROOM_CLOSED | Room closed |
| 4 | KICKED | Optional |

---

## R14. ROOM_GPU_FRAME Flags (u32 payload flags)

| Bit | Name | Meaning |
|---:|---|---|
| 0 | KEYFRAME | This frame is a keyframe |
| 1 | HAS_CRC32 | crc32_cmd present |
| 2..31 | Reserved | |

---

## R15. App Metadata TLV Types (APP_MANIFEST_GET_RESP TLVBlock)

All types are u16

| tlv_type | Name |
|---:|---|
| 0x1000 | APP_META_CORE |
| 0x1001 | APP_META_TICK |
| 0x1002 | APP_META_GPU |
| 0x1003 | APP_META_RNG |
| 0x1004 | APP_META_STATE_HASH |
| 0x1010 | APP_META_ENTRYPOINT_HINTS |

---

## R16. App Flags (APP_META_CORE.app_flags u32)

| Bit | Name |
|---:|---|
| 0 | APPF_SUPPORTS_MODE_A_GPU |
| 1 | APPF_SUPPORTS_MODE_B_LOCKSTEP |
| 2 | APPF_SUPPORTS_SAVES |
| 3 | APPF_REQUIRES_AUTH |
| 4 | APPF_DETERMINISTIC_CORE |
| 5..31 | Reserved |

---

## R17. RNG IDs (APP_META_RNG.rng_id u32)

| Value | Name |
|---:|---|
| 0 | NONE (invalid for deterministic Mode B) |
| 1 | xorshift32 |
| 2 | xorshift64* |
| 3 | pcg32 (reserved/optional later) |
| 4.. | Reserved |

---

## R18. State Hash Algorithms (APP_META_STATE_HASH.hash_alg_id u32)

| Value | Name |
|---:|---|
| 1 | CRC32 (IEEE) |
| 2.. | Reserved |

---

## R19. UnityGPU Stream Versions (APP_META_GPU.gpu_stream_version u16)

| Value | Name |
|---:|---|
| 0 | UnityGPU Stream v0 |
| 1.. | Reserved |

---

## R20. UnityGPU Stream v0 Opcodes (u8)

Command encoding: u8 opcode, u8 len, bytes[len] payload

### Standard opcodes (0x00–0x7F)
| Opcode | Name | Len | Notes |
|---:|---|---:|---|
| 0x01 | SET_VIEWPORT | 12 | width,height,cam_x,cam_y,flags |
| 0x02 | CLEAR | 2 | color_index, flags |
| 0x03 | PALETTE_SET | 6 | entry,r,g,b,flags |
| 0x10 | SPRITE_DRAW | 16 | sheet,idx,x,y,sx,sy,color,flags |
| 0x11 | RECT_FILL | 10 | x,y,w,h,color,flags |
| 0x12 | TILEMAP_BLIT | 16 | tilemap,tileset,srcx,srcy,w,h,dstx,dsty |
| 0x13 | TEXT_DRAW | 8+N | font,x,y,color,flags,text_len,text |

### System/reserved (0xF0–0xFF)
| Opcode | Name | Len |
|---:|---|---:|
| 0xFE | NOP | 0 |
| 0xFF | END_FRAME | 0 |

### Extension ranges
- 0x80–0xBF experimental
- 0xC0–0xEF platform-specific (future negotiation)
