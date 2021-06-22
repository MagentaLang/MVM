#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// logging/debug functions
void debug(char text[]) { printf("→ %s", text); }
void info(char text[]) { printf("\033[32;1m→ \033[m %s", text); }
void warn(char text[]) { printf("\033[33;1m→ \033[m %s", text); }
void err(char text[]) { printf("\033[31;1m→ \033[m %s", text); }

// string reverse
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

// get nth byte of a bytes
unsigned long nth_byte(int n, unsigned long bytes) {
	return (bytes >> (8 * n));
}

// returns bigger unsigned long
unsigned long maxul(unsigned long a, unsigned long b) {
	if (a > b)
		return a;
	else
		return b;
}
