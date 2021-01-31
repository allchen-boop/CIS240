/* 
 * stack.c (based off of queue.c)
 */

#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// tracks size of stack (can't fix stack size)
int size = 0;

void init_stack (stack *stack) {
	stack -> top = NULL;
}

// place element on at top of stack
void push (stack *stack, fraction frac) {
	stack_elt *elt;

	// allocate new stack element
	elt = malloc(sizeof(*elt));

	// if malloc fails
	if (elt == NULL) {
		printf("STACK ERROR\n");
		exit(1);
	}

	// allocate space to store fraction value
	elt -> value = malloc(sizeof(fraction));

	if (elt -> value == NULL) {
		printf("STACK ERROR\n");
		exit(1);
	}

	elt -> value -> num = frac.num;
	elt -> value -> denom = frac.denom;

	// next value is the previous value (which is at the top)
	elt -> next = stack -> top; 

	// top is the next element
	stack -> top = elt;

	// pushing an element will increase stack size
	size ++;
}


// remove top element and return value
fraction pop (stack *stack) {
	stack_elt *elt;
	fraction frac;

	// stack is not empty
	if (stack -> top) {

		// current element off the stack
		elt = stack -> top;
		frac = *(elt -> value);

		// removing element (top is now the next element)
		stack -> top = elt -> next;

		free (elt -> value);
		free (elt);

		// removing an element will decrease size of stack
		size --;
	}
	
	return frac;
}

// return top element
fraction peek (stack *stack) {
	stack_elt *elt;
	fraction frac;

	if (stack -> top) {
		elt = stack -> top;
		frac = *(elt -> value);
	}

	return frac;
}

void clear(stack *stack) {
	while (stack -> top) {
		pop (stack);
	}
} 

int stack_size() {
	return size;
}
