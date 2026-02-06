# App Metadata TLVs v0

This document defines the authoritative binary metadata TLVs carried in `APP_MANIFEST_GET_RESP` TLVBlock.

Goal: allow UnityXX clients to avoid JSON parsing for netplay/GPU parameters and other hot-path settings.

Authoritative references:
- TLV type registry: `registries-v0.2.md` (App Metadata TLV Types)
- Field meanings and constraints: maintained here

## Precedence
If a value appears in both:
- TLVs are authoritative
- `manifest_extra_json` is advisory/UI-only

## TLVs (v0)
- APP_META_CORE
- APP_META_TICK
- APP_META_GPU
- APP_META_RNG
- APP_META_STATE_HASH
- APP_META_ENTRYPOINT_HINTS

## TODO
Paste the full TLV schema definitions here (field layouts, caps, required/optional rules).
