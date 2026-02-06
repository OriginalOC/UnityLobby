.DEFAULT_GOAL := compat-check
.PHONY: vectors compat-sync compat-check

vectors:
	./tools/hexframes_to_bin.py test-vectors/frames --all

compat-sync:
	python3 tools/sync_compat_tables.py

compat-check:
	python3 tools/sync_compat_tables.py --check
