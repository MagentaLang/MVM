#include <stdio.h>

void info(char text[]) { printf("\033[32;1m→ \033[m %s\n", text); }
void warn(char text[]) { printf("\033[33;1m→ \033[m %s\n", text); }
void err(char text[]) { printf("\033[31;1m→ \033[m %s\n", text); }

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
		// create program stack
		int counter = 0;
		char pstack[size];
		if (fgets(pstack, size, f) == NULL) {
			err("fatal: error reading file");
			return -1;
		}
		// print program stack
		while (counter < size) {
			printf("%x ", pstack[counter]);
			counter++;
		}
		printf("\n");

		fclose(f);
	}

	return 0;
}
