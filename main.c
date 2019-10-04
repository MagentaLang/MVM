#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void info(char text[]) { printf("\033[32;1m→ \033[m %s\n", text); }
void warn(char text[]) { printf("\033[33;1m→ \033[m %s\n", text); }
void err(char text[]) { printf("\033[31;1m→ \033[m %s\n", text); }
void strrev(char *str);

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
		stack *sstack = initstack(1024);
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
					case 0x00: {
						mode = opush;
						stack_push(sstack, 0);
					} break;

					case 0x02: {
						char str[1024];

						for (int i = 1; stack_peek(sstack) != 0; i++)
							str[i] = stack_pop(sstack);

						strrev(str);
						printf("%s\n", str);
					} break;

					case 0x08: case 0x09: case 0x0b: case 0x0c: {
						char a = pstack[counter + 1];
						char b = pstack[counter + 2];
						counter += 2;

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
						printf("          [%d] 0x%x <--\n", counter + 1, c);
						return -1;
					}
				} break;

				case opush: switch (c) {
					case 0x00: {
						mode = normal;
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

void strrev(char *str) {
	char *start = str;
	char *end = start + strlen(str) - 1;
	char temp;

	while (end > start) {
		temp = *start;
		*start = *end;
		*end = temp;

		++start;
		--end;
	}
}
