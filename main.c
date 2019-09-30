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

		int c;
		while ((c = fgetc(f)) != EOF) {
			printf("%x ", c);
		}
		printf("\n");

		fclose(f);
	}

	return 0;
}
