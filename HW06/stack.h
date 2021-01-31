/*
 * stack.h
 */

typedef struct {
	int num;
	int denom;

}	fraction;


typedef struct stack_elt_tag {
	fraction *value;
	struct stack_elt_tag *next;

} 	stack_elt;


typedef struct {
	stack_elt *top;

} 	stack;

void init_stack (stack *stack);
void clear(stack *stack);
void push (stack *stack, fraction frac);

fraction pop (stack *stack); 
fraction peek (stack *stack);

int stack_size();