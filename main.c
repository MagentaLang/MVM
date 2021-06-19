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
	opush,
};

int main(int argc, char *argv[]) {
	if (argc <= 1) {
	} else {
		FILE *f = fopen(argv[1], "r");

		if (f == NULL) {
			err("fatal: error opening file");
			exit(EXIT_FAILURE);
		}

		// get file size
		fseek(f, 0, SEEK_END);
		int size = ftell(f) + 1;
		printf("size: %d\n", size);
		fseek(f, 0, SEEK_SET);

		// create stacks
		char pstack[size];
		char mstack[1024];
		stack *sstack = initstack(1024);
		// create program stack
		int counter = 0;
		if (fgets(pstack, size, f) == NULL) {
			err("fatal: error reading file");
			exit(EXIT_FAILURE);
		}

		// print program stack
		enum mode mode = normal;
		while (counter < size) {
			char c = pstack[counter];

			switch (mode) {
				case normal: switch (c) {
					case 0x00: {
						mode = opush;
					} break;

					case 0x02: {
						char str[1024];

						for (int i = 1; stack_peek(sstack) != 0; i++)
							str[i] = stack_pop(sstack);

						strrev(str);
						printf("%s", str);
					} break;

					case 0x08: case 0x09: case 0x0b: case 0x0c: {
						char a = stack_pop(sstack);
						char b = stack_pop(sstack);

						if (c == 0x08) stack_push(sstack, a + b);
						if (c == 0x09) stack_push(sstack, a - b);
						if (c == 0x0b) stack_push(sstack, a * b);
						if (c == 0x0c) stack_push(sstack, a / b);
					} break;

					case 0x13: {
						char c = stack_pop(sstack);
						char str[4];

						sprintf(str, "%d", c);
						strrev(str);

						for (int i = 0; str[i] == 0; i++)
							stack_push(sstack, i);
					} break;

					default: {
						err("fatal: invalid opcode");
						printf("\n          [%d] 0x%x <--\n", counter + 1, c);
						exit(EXIT_FAILURE);
					}
				} break;

				case opush:
					printf("%x\n", c);switch (c) {
					case 0x00: {
						mode = normal;
						stack_dump(sstack);
					} break;

					case 0x1A: {
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
