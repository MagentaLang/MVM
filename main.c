#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void info(char text[]) { printf("\033[32;1m→ \033[m %s\n", text); }
void warn(char text[]) { printf("\033[33;1m→ \033[m %s\n", text); }
void err(char text[]) { printf("\033[31;1m→ \033[m %s\n", text); }

enum mode {
	normal,
	opush,
};

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("usage: mvm file\n");
	} else {
		FILE *f = fopen(argv[1], "r");

		if (f == NULL) {
			err("fatal: error opening file");
			return -1;
		}

		// get file size
		fseek(f, 0, SEEK_END);
		int size = ftell(f) + 1;
		fseek(f, 0, SEEK_SET);

		// create stacks
		char pstack[size];
		char mstack[1024];
		stack *sstack = newStack(1024);
		// create program stack
		int counter = 0;
		if (fgets(pstack, size, f) == NULL) {
			err("fatal: error reading file");
			return -1;
		}

		// print program stack
		enum mode mode = normal;
		while (counter < size) {
			char c = pstack[counter];

			switch (mode) {
				case normal: switch (c) {
					case 0x0: {
						mode = opush;
					} break;

					case 0x3: {
						printf("hi");
					} break;

					default: {
						err("fatal: invalid opcode");
						printf("          [%d] 0x%x <--\n", counter + 1, c);
						return -1;
					}
				} break;

				case opush: switch (c) {
					case 0x0: {
						mode = normal;
					} break;

					default: {
						push(sstack, c);
					}
				} break;
			}

			counter++;
		}
		printf("\n");

		fclose(f);
	}

	return 0;
}
