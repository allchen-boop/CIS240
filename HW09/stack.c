/* 
 * stack.c (modified my rpn)
 */

#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_stack(stack *stack) {
	stack -> top = NULL;
}

void push(stack *stack, int num) {

	stack_elt *elt;

	// allocate new stack element
	elt = malloc(sizeof(*elt));

	// if malloc fails
	if (elt == NULL) {
		printf("STACK ERROR\n");
		exit(1);
	}

	elt -> value = num;

	// next value is the previous value (which is at the top)
	elt -> next = stack -> top; 

	// top is the next element
	stack -> top = elt;
}

int pop(stack *stack) {

	stack_elt *elt;
	int popped = 0;

	// stack is not empty
	if (stack -> top) {

		// current element off the stack
		elt = stack -> top;
		popped = elt -> value;

		// removing element (top is now the next element)
		stack -> top = elt -> next;

		free (elt);
	}

	return popped;
}

// return top element
int peek (stack *stack) {
	stack_elt *elt;
	int peeked = 0;

	if (stack -> top) {
		elt = stack -> top;
		peeked = elt -> value;
	}

	return peeked;
}


void clear(stack *stack) {
	while (stack -> top) {
		pop(stack);
	}
} 
