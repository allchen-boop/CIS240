/*
 * stack.h (same as my rpn)
 */

// stack struct
typedef struct stack_elt_tag {
	int value;
	struct stack_elt_tag *next;
} stack_elt;

typedef struct {
	stack_elt *top;
} stack;

void init_stack(stack *stack);
void push(stack *stack, int value);
int pop(stack *stack);
int peek (stack *stack);
void clear(stack *stack);

