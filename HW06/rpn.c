/*
 * rpn.c
 */

#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

/* 
 * CALCULATOR HELPER FUNCTIONS
 */

int gcd (int a, int b) {
	if (a == 0) {
		return b;
	}
	return gcd (b % a, a);
}

fraction simplify (fraction frac) {
	fraction frac_simplify;

	int num_val = frac.num;
	int denom_val = frac.denom;
	int gcd_val = gcd (num_val, denom_val);

	frac_simplify.num = num_val / gcd_val;
	frac_simplify.denom = denom_val / gcd_val;

	return frac_simplify;
}


fraction add (fraction frac_one, fraction frac_two) {
	fraction frac_sum;

	if (frac_one.denom != frac_two.denom) {
		int sum_num = frac_one.num * frac_two.denom + frac_two.num * frac_one.denom;
		int sum_denom = frac_one.denom * frac_two.denom;

		frac_sum.num = sum_num;
		frac_sum.denom = sum_denom;

		return simplify (frac_sum);
	}

	frac_sum.num = frac_one.num + frac_two.num;
	frac_sum.denom = frac_one.denom;

	return simplify (frac_sum);
}


fraction sub (fraction frac_one, fraction frac_two) {
	fraction frac_diff;

	frac_diff.num = -frac_one.num;
	frac_diff.denom = frac_one.denom;
	
	return add (frac_two, frac_diff);
}


fraction mult (fraction frac_one, fraction frac_two) {
	fraction frac_product;

	int product_num = frac_one.num * frac_two.num;
	int product_denom = frac_one.denom * frac_two.denom;

	frac_product.num = product_num;
	frac_product.denom = product_denom;

	return simplify (frac_product);	
}


fraction divide (fraction frac_one, fraction frac_two) {
	fraction frac_reciprocal;

	frac_reciprocal.num = frac_one.denom;
	frac_reciprocal.denom = frac_one.num;

	return mult (frac_two, frac_reciprocal);	
}


int main ()
{
	char input[MAX_LINE_LENGTH];

	char string_one[MAX_LINE_LENGTH];
	char string_two[MAX_LINE_LENGTH];

	// initializing the stack
	stack theStack;
	init_stack (&theStack);

	fraction frac_input;
	fraction frac_top;
	fraction frac_next;
	fraction frac_result;
	fraction frac_peek;

	// for loop	
	int i;

	while (1) {

		fgets (input, MAX_LINE_LENGTH, stdin);

		// convert user input all to lower case
		for (i = 0; i < strlen (input); i++) {
			input[i] = tolower (input[i]);
		}

		// check for invalid command
		if (sscanf (input, "%s %s", string_one, string_two) == 2) {
			printf ("INVALID COMMAND\n");

		// operation after input
		} else if (sscanf (input, "%d%s", &frac_input.num, string_one) == 2) {
			printf ("INVALID COMMAND\n");

		// decimal integer input
		} else if (sscanf (input, "%d", &frac_input.num) == 1) {
			frac_input.denom = 1;
			
			// push to top of stack
			push (&theStack, frac_input);
			frac_peek = peek (&theStack);

			printf ("%d/%d\n", frac_peek.num, frac_peek.denom);

		// operations
		} else if (sscanf (input, "%s", string_one) == 1) {

			// add
			if (strcmp (string_one, "+") == 0) {
				if (stack_size() < 2) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					frac_next = pop (&theStack);
					frac_result = add (frac_top, frac_next);

					push (&theStack, frac_result);
					frac_peek = peek (&theStack);

					if (frac_peek.denom < 0) {
						printf ("%d/%d\n", -frac_peek.num, -frac_peek.denom);
					} else {
						printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
					}
				} 
				
			// subtract	
			} else if (strcmp (string_one, "-") == 0) {
				if (stack_size() < 2) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					frac_next = pop (&theStack);
					frac_result = sub (frac_top, frac_next);

					push (&theStack, frac_result);
					frac_peek = peek (&theStack);

					if (frac_peek.denom < 0) {
						printf ("%d/%d\n", -frac_peek.num, -frac_peek.denom);
					} else {
						printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
					}
				}
		
			// multiply	
			} else if (strcmp (string_one, "*") == 0) {
				if (stack_size() < 2) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					frac_next = pop (&theStack);
					frac_result = mult (frac_top, frac_next);

					push (&theStack, frac_result);
					frac_peek = peek (&theStack);

					if (frac_peek.denom < 0) {
						printf ("%d/%d\n", -frac_peek.num, -frac_peek.denom);
					} else {
						printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
					}
				}
			
			// divide
			} else if (strcmp (string_one, "/") == 0) {
				if (stack_size() < 2) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					frac_next = pop (&theStack);
					frac_result = divide (frac_top, frac_next);

					if (frac_result.denom == 0) {
						printf ("DIVIDE BY ZERO\n");
						clear (&theStack);
						exit (1);
					}

					push (&theStack, frac_result);
					frac_peek = peek (&theStack);

					if (frac_peek.denom < 0) {
						printf ("%d/%d\n", -frac_peek.num, -frac_peek.denom);
					} else {
						printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
					}
				}
			
			// dup
			} else if (strcmp (string_one, "dup") == 0) {
				if (stack_size() < 1) {
					printf ("STACK ERROR\n");
				} else {
					push (&theStack, (peek (&theStack)));
					frac_peek = peek (&theStack);

					printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
				}
			
			// swap
			} else if (strcmp (string_one, "swap") == 0) {
				if (stack_size() < 2) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					frac_next = pop (&theStack);
					
					push (&theStack, frac_top);
					push (&theStack, frac_next);
					frac_peek = peek (&theStack);

					printf ("%d/%d\n", frac_peek.num, frac_peek.denom);
				}
			
			// quit
			} else if (strcmp (string_one, "quit") == 0) {
				clear (&theStack);
				exit(1);
			
			//drop
			} else if (strcmp(string_one, "drop") == 0) {
				if (stack_size() < 1) {
					printf ("STACK ERROR\n");
				} else {
					frac_top = pop (&theStack);
					printf("%d/%d\n", frac_top.num, frac_top.denom);
				}
			
			//clear
			} else if (strcmp (string_one, "clear") == 0) {
				clear (&theStack);

			} else {
				printf ("INVALID COMMAND\n");
			}

		} else {
			printf ("INVALID COMMAND\n");
		}
	} 
}

