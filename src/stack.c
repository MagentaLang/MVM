#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack *initstack(int capacity) {
	stack *p = (stack*)malloc(sizeof(stack));
	if (p == NULL) {
		printf("Did Not Allocate Stack.");
		exit(EXIT_FAILURE);
	}

	p->maxsize = capacity;
	p->top = -1;
	p->items = (char*)malloc(sizeof(char) * capacity);

	return p;
}

char stack_size(stack *p) {
	return p->top + 1;
}

char stack_empty(stack *p) {
	return p->top == -1;
}

char stack_full(stack *p) {
	return p->top == p->maxsize - 1;
}

void stack_push(stack *p, char x) {
	if (stack_full(p)) {
		printf("OverFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	p->items[++p->top] = x;
}

char stack_peek(stack *p) {
	if (!stack_empty(p)) {
		return p->items[p->top];
	} else {
		exit(EXIT_FAILURE);
	}
}

char stack_pop(stack *p) {
	if (stack_empty(p)) {
		printf("UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	return p->items[p->top--];
}
