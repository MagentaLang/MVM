#ifndef STACK_H
#define STACK_H

typedef struct {
	int maxsize;
	int top;
	char *items;
} stack;

stack* newStack(int capacity);
char size(stack *p);
char isEmpty(stack *p);
char isFull(stack *p);
void push(stack *p, char x);
char peek(stack *p);
char pop(stack *p);

#endif
