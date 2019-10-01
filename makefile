CC=gcc
CFLAGS=-Isrc

build:
	@[ -d ".bin" ] || mkdir .bin
	@echo "==> building"
	$(CC) $(CFLAGS) src/stack.c main.c -o .bin/mvm

test: build
	@echo "==> running test"
	@./.bin/mvm test.magenta

run: build
	@echo "==> running"
	@./.bin/mvm
