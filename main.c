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
		int width = 1; // byte width
		stack *sstack = initstack(1024); // stack
		// create program stack
		int counter = 0;
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

		// execute program
		enum mode mode = normal;
		while (counter < size) {
			char c = bytecode[counter];

			switch (mode) {
				case normal: switch (c) {
					case 0x00: {
						mode = push;
					} break;

					case 0x02: {
						for (int i = 0; stack_peek(sstack) != 0x00; i++)
							putchar(stack_pop(sstack));
					} break;

					case 0x04: {
						char c = stack_pop(sstack);
						char str[4];

						sprintf(str, "%d", c);
						strrev(str);

						for (int i = 0; str[i] != 0x00; i++)
							stack_push(sstack, str[i]);
					} break;

					case 0x0A: case 0x0B: {
						char a = stack_pop(sstack);
						char b = stack_pop(sstack);

						if (c == 0x08) stack_push(sstack, a + b);
						if (c == 0x09) stack_push(sstack, a - b);
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
					switch (c) {
					case 0x00: {
						// TODO: check for consecutive null bytes the length of
						// memory width
						mode = normal;
					} break;

					case 0xFF: {
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
