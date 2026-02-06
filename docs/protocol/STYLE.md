# Protocol Style Guide (UnityLobby)

This guide keeps UnityLink/UnityGPU docs consistent, readable, and implementable across MCU-class clients.

## Normative language
Use these terms with intent:
- **MUST / MUST NOT**: required for compliance
- **SHOULD / SHOULD NOT**: strongly recommended; valid exceptions exist
- **MAY**: optional

Avoid “can” when you mean MUST/MAY.

## Structure of a spec section
For each feature/message:
1) Purpose
2) Payload layout (byte-accurate)
3) Rules (ordering, caps, error codes)
4) Backpressure / drop policy (if streaming or events)
5) Compatibility notes (TLVs, future versions)

## Wire format conventions
- All integers are little-endian unless stated otherwise
- Every variable-length field must have:
  - a length field
  - an explicit maximum in the spec
- Avoid JSON on hot paths. Prefer fixed structs or TLVs.
- Unknown TLVs/opcodes MUST be skippable (length-delimited).

## Tables and registries
- Any enum or ID needs a registry entry
- Never reuse an ID for a different meaning
- Prefer sparse registries that can grow without collisions

## Error handling
- Responses use status codes (ERR_*)
- If status != 0, payload MUST begin with:
  - `varstr error_message`
- If extra detail is needed, add a TLV (do not change base layout in-place)

## Streaming rules
If a message streams via FLAG_MORE:
- Define exactly what “complete” means
- Define assembly keys (request_id, room_id, frame_no, etc.)
- Define timeouts / stale-drop behavior
- Define backpressure and drop policy (deterministic)

## Ordering rules
Explicitly state ordering guarantees:
- within a request_id stream
- within a room event stream
- for chunked frame assembly

## Examples and test vectors
Any new message family should include:
- at least one hexdump example in [test-vectors/frames/](test-vectors/frames/)
- any required sha/crc vectors in [test-vectors/](test-vectors/)

## Versioning
- Keep v0.2 frozen; use errata for fixes
- v0.3+ features should be capability/TLV gated
- If a breaking change is unavoidable, require a migration plan and a new major version

## Formatting
- Prefer short paragraphs
- Use code blocks for structs
- Use bullet lists for rules
- Avoid ambiguous prose
