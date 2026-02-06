# UnityLobby Repo Layout (on-disk)

This document describes the recommended on-disk layout under `storage.root`.
Do not commit the repo data directory to Git.

## Objects (content addressed)
Objects are immutable and stored by sha256 hex.

Example:
repo/objects/aa/bb/<sha256hex>

Where:
- aa = first 2 hex chars of sha256
- bb = next 2 hex chars of sha256

## Apps and manifests
Recommended:
repo/apps/<app_id_hex>/versions/<version>/manifest.bin
repo/apps/<app_id_hex>/versions/<version>/manifest.json (optional for tooling)

Manifests should be rebuildable from source of truth, and indexes should be rebuildable from manifests.

## Catalog index
repo/index/catalog.bin

Prebuilt listing for fast CATALOG_LIST responses.

## Saves
repo/saves/<user_id_hex>/<app_id_hex>/<slot>.bin
repo/saves/<user_id_hex>/<app_id_hex>/<slot>.meta (timestamps, size, optional hashes)

Saves are committed atomically after SAVE_PUT_COMMIT.

## Rooms
Rooms are typically ephemeral. If persisted:
repo/rooms/<room_id_hex>/ (optional)

## Notes
- Because objects are immutable and content addressed, caches and proxies can safely store them without coherence protocols beyond presence checks.
