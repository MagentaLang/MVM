#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "util.h"

void usage() {
	printf("usage: mvm file\n");
}

enum mode {
	normal,
	push,
};

int main(int argc, char *argv[]) {
	if (argc <= 1) {
	} else {
		FILE *f = fopen(argv[1], "rb");

		if (f == NULL) {
			err("fatal: error opening file");
			exit(EXIT_FAILURE);
		}

		// get file size
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);

		char mstack[256]; // memory
		int mpointer = 0x01;
		int registerA = 0x01;
		int registerB = 0x01;
		int width = 1; // byte width
		stack *sstack = initstack(1024); // stack
		// create program stack
		int counter = 3;
		char bytecode[size]; // bytecode
		size_t ret_code = fread(bytecode, sizeof(char), size, f);
		if (ret_code == size) {
			// good :)
		} else {
			if (feof(f)) {
				err("fatal: unexpected end of file\n");
				exit(EXIT_FAILURE);
			} else if (ferror(f)) {
				err("fatal: error reading file\n");
				exit(EXIT_FAILURE);
			}
		}

		if (bytecode[0] != 'M' || bytecode[1] != 'V' || bytecode[2] != 'M') {
			err("fatal: file is not magenta bytecode\n");
			exit(EXIT_FAILURE);
		}

		// execute program
		enum mode mode = normal;
		while (counter < size) {
			char c = bytecode[counter];

			switch (mode) {
				case normal: switch (c) {
					case 0x1F: break; // noop

					case 0x00: {
						mode = push;
					} break;

					case 0x01: {
						for (int i = 0; i < width; i++)
							stack_push(sstack, bytecode[counter + i + 1]);

						counter += width;
					} break;

					case 0x02: case 0x03: {
						unsigned long long int bytes = 0x00;
						stack_pop_width(sstack, width, &bytes);

						if (c == 0x02) registerA = bytes;
						if (c == 0x03) registerB = bytes;
					} break;

					case 0x16: { // debugging log command
						for (int i = 0; stack_peek(sstack) != 0x00; i++)
							putchar(stack_pop(sstack));
					} break;

					case 0x04: {
						unsigned long long int byte;
						stack_pop_width(sstack, width, &byte);
						char str[11];

						sprintf(str, "%llu", byte);
						strrev(str);

						for (int i = 0; str[i] != 0x00; i++)
							stack_push(sstack, str[i]);
					} break;

					case 0x0A: case 0x0B: {
						if (c == 0x0A) stack_push(sstack, registerA + registerB);
						if (c == 0x0B) stack_push(sstack, registerA - registerB);
					} break;

					case 0x12: width = 1; break;
					case 0x13: width = 2; break;
					case 0x14: width = 4; break;
					case 0x15: width = 8; break;

					default: {
						err("fatal: invalid opcode");
						printf("\n          [%d] 0x%x <--\n", counter + 1, c);
						exit(EXIT_FAILURE);
					}
				} break;

				case push:
					switch ((int)c) {
					case 0x00: {
						mode = normal;
					} break;

					case 0xffffffff: {
						stack_push(sstack, 0);
					} break;

					default: {
						stack_push(sstack, c);
					}
				} break;
			}

			counter++;
		}

		fclose(f);
	}

	return 0;
}
