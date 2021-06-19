#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#include "stack.h"

// creates a stack
stack *initstack(int capacity) {
	stack *p = (stack*)malloc(sizeof(stack));
	if (p == NULL) {
		err("Could Not Allocate Stack\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	p->maxsize = capacity;
	p->top = -1;
	p->items = (char*)malloc(sizeof(char) * capacity);

	return p;
}

// returns the current size of the stack
char stack_size(stack *p) {
	return p->top + 1;
}

// checks if stack is full
char stack_full(stack *p) {
	return p->top == p->maxsize - 1;
}

// pushes an element to top of stack
void stack_push(stack *p, char x) {
	if (stack_full(p)) {
		err("OverFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	p->items[++p->top] = x;
}

// checks if stack is full
char stack_empty(stack *p) {
	return p->top == -1;
}

// removes the top element from the stack and returns it
char stack_pop(stack *p) {
	if (stack_empty(p)) {
		err("Stack UnderFlow\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}

	return p->items[p->top--];
}

// returns the top element from the stack and returns it
char stack_peek(stack *p) {
	if (!stack_empty(p)) {
		return p->items[p->top];
	} else {
		err("Stack Empty\nProgram Terminated\n");
		exit(EXIT_FAILURE);
	}
}

// prints indexed bytes from a stack
void stack_dump(stack *p) {
	if (stack_empty(p)) {
		debug("Dumping Stack: empty\n");
	} else {
		debug("Dumping Stack: ");
		printf("length of %d\n", p->top + 1);
		for (int i = 0; i < p->top + 1; i++) {
			printf("%d: %x\n", i, p->items[i]);
		}
	}
}
