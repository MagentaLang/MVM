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
		unsigned size = ftell(f);
		fseek(f, 0, SEEK_SET);

		unsigned char mstack[256]; // memory
		unsigned int mpointer = 0x01;
		unsigned long long int registerA = 0x01;
		unsigned long long int registerB = 0x01;
		int width = 1; // byte width
		stack *sstack = initstack(1024); // stack
		// create program stack
		int counter = 3;
		unsigned char bytecode[size]; // bytecode
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
			unsigned char c = bytecode[counter];

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
						// width is at least one
						unsigned long long int a = nth_byte(0, registerA);
						unsigned long long int b = nth_byte(0, registerB);

						if (width == 1) {
							stack_push(sstack, c == 0x0A ? (unsigned char)a + (unsigned char)b : (unsigned char)a - (unsigned char)b);
							break;
						}

						// width is at least two
						a = a | nth_byte(1, registerA) << 8;
						b = b | nth_byte(1, registerB) << 8;

						if (width == 2) {
							unsigned short sum = c == 0x0A ? ((unsigned short)a + (unsigned short)b) : ((unsigned short)a - (unsigned short)b);
							stack_push(sstack, nth_byte(1, sum));
							stack_push(sstack, nth_byte(0, sum));
							break;
						}

						if (width == 4) {

						}

						if (width == 8) {

						}
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
					if (c == 0x00) {
						mode = normal;
					} else if (c >= 0xFF) {
						stack_push(sstack, 0x00);
					} else {
						stack_push(sstack, c);
					}
					break;
			}

			counter++;
		}

		fclose(f);
	}

	return 0;
}
