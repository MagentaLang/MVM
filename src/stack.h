#ifndef STACK_H
#define STACK_H

typedef struct {
	int maxsize;
	int top;
	char *items;
} stack;

stack *initstack(int capacity);
char stack_size(stack *p);
char stack_empty(stack *p);
char stack_full(stack *p);
void stack_push(stack *p, char x);
char stack_peek(stack *p);
char stack_pop(stack *p);
void stack_dump(stack *p);

#endif
