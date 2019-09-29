CC=gcc
CFLAGS=-I.

build:
	@[ -d ".bin" ] || mkdir .bin
	@echo "==> building"
	$(CC) $(CFLAGS) main.c -o .bin/mvm

test: mvm
	@echo "==> no tests specified"

run: mvm
	@echo "==> running"
	@./.bin/mvm
