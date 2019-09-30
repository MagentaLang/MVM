CC=gcc
CFLAGS=-I.

build:
	@[ -d ".bin" ] || mkdir .bin
	@echo "==> building"
	$(CC) $(CFLAGS) main.c -o .bin/mvm

test: build
	@echo "==> no tests specified"

run: build
	@echo "==> running"
	@./.bin/mvm
