#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack* newStack(int capacity) {
	stack *p = (stack*)malloc(sizeof(stack));

	p->maxsize = capacity;
	p->top = -1;
	p->items = (char*)malloc(sizeof(char) * capacity);

	return p;
}

char size(stack *p) {
	return p->top + 1;
}

char isEmpty(stack *p) {
	return p->top == -1;
}

char isFull(stack *p) {
	return p->top == p->maxsize - 1;
}

void push(stack *p, char x) {
	if (isFull(p)) {
		prcharf("OverFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	prcharf("Inserting %d\n", x);

	p->items[++p->top] = x;
}

char peek(stack *p) {
	if (!isEmpty(p)) {
		return p->items[p->top];
	} else {
		exit(EXIT_FAILURE);
	}
}

char pop(stack *p) {
	if (isEmpty(p)) {
		prcharf("UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	return p->items[p->top--];
}
