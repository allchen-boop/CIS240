/* 
 * jc.c :  main j compiler 
 */

#include "token.h"
#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main (int argc, char **argv) {

    int defun_tracker = 0;
    int compare_tracker = 0;
    int if_tracker = 0;
    int endif_tracker = 0;

    int literal_value;
    int const_value;
    int hiconst_value;

    int arg_pos;

    FILE *input_file;   // j file
    FILE *output_file;  // asm file

    char *label;        // for branch labels
    char *asm_file;     // output asm file

    input_file = fopen(argv[1], "r");

    if (!input_file) {
        printf("UNABLE TO OPEN J FILE\n");
        exit(1);
    }  

    label = malloc(sizeof(char) * 50);
    asm_file = malloc(sizeof(char) * 50);

    sscanf(argv[1], "%[^.].j", label);
    sscanf(argv[1], "%[^.].j", asm_file);

    // writing asm file
    strcat(asm_file, ".asm");
    output_file = fopen(asm_file, "w");

    if (!output_file) {
        free(label);
        free(asm_file);
        fclose(input_file);
        printf("UNABLE TO OPEN ASM FILE\n");
        exit(1);
    }

    token *theToken;
    stack *if_stack; 
    stack *elseif_stack; 

    theToken = malloc(sizeof(token));
    if_stack = malloc(sizeof(stack));
    elseif_stack = malloc(sizeof(stack));

    init_stack (if_stack);
    init_stack (elseif_stack);

    while(!feof(input_file)) {
        read_token(theToken, input_file);

        switch (theToken->type)  {

            case DEFUN : 
                if (!read_token(theToken, input_file)) {
                    fprintf(output_file, "\n\t\t.CODE\n");
                    fprintf(output_file, "\t\t.FALIGN\n");
                    fprintf(output_file, "\n%s\n", theToken -> str);

                    fprintf(output_file, "\n\t;; prologue\n");

                    fprintf(output_file, "\tSTR R7, R6, #-2\n");
                    fprintf(output_file, "\tSTR R5, R6, #-3\n");
                    fprintf(output_file, "\tADD R6, R6, #-3\n");
                    fprintf(output_file, "\tADD R5, R6, #0\n");
 

                    fprintf(output_file, "\n\t;; function body\n");

                    defun_tracker = 1;
                }

                break;

            case IDENT :

                if (defun_tracker) {
                    fprintf(output_file, "\n\t;; calling %s\n", theToken -> str);
                    fprintf(output_file, "\tJSR %s\n", theToken -> str);
                    fprintf(output_file, "\tADD R6, R6, #-1\n");
                } else {
                    printf("ERROR: IDENT WITHOUT DEFUN\n");
                }

                break;

            case RETURN :

                if (defun_tracker) {
                    fprintf(output_file, "\n\t;; epilogue\n");

                    fprintf(output_file, "\tLDR R7, R6, #0\n");
                    fprintf(output_file, "\tSTR R7, R5, #2\n");
                    fprintf(output_file, "\tADD R6, R5, #0\n");
                    fprintf(output_file, "\tLDR R5, R6, #0\n");
                    fprintf(output_file, "\tLDR R7, R6, #1\n");
                    fprintf(output_file, "\tADD R6, R6, #3\n");
                    fprintf(output_file, "\tRET\n");

                    defun_tracker = 0;
                }

                break;

            case PLUS  :

                fprintf(output_file, "\n\t;; adding\n");
                
                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tADD R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n");

                break; 

            case MINUS  :

                fprintf(output_file, "\n\t;; subtracting\n");

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tSUB R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n");

                break;      

            case MUL :

                fprintf(output_file, "\n\t;; multiplying\n"); 

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tMUL R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n");

                break;      

            case DIV :

                fprintf(output_file, "\n\t;; dividing\n"); 

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tDIV R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n");

                break;     

            case MOD :

                fprintf(output_file, "\n\t;; mod\n"); 

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tMOD R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n"); 

                break;

            case AND :

                fprintf(output_file, "\n\t;; and\n"); 

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tAND R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n"); 

                break;      

            case OR :

                fprintf(output_file, "\n\t;; or\n"); 

                fprintf(output_file,"\tLDR R0 R6 #0\n");
                fprintf(output_file,"\tLDR R1 R6 #1\n"); 
                fprintf(output_file,"\tOR R2 R0 R1\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");
                fprintf(output_file,"\tSTR R2 R6 #0\n"); 

                break;      

            case NOT :

                fprintf(output_file, "\n\t;; not\n"); 

                fprintf(output_file,"\tLDR R1 R6 #0\n");
                fprintf(output_file,"\tNOT R1 R1\n");
                fprintf(output_file,"\tSTR R1 R6 #0\n"); 

                break; 

            case LT :

                 fprintf(output_file, "\n\t;; less than\n");
       
                fprintf(output_file, "\tLDR R0, R6, #0\n"); 
                fprintf(output_file, "\tLDR R1, R6, #1\n"); 
                fprintf(output_file, "\tADD R6, R6, #1\n"); 
                fprintf(output_file, "\tCMP R0 R1\n");  

                // if false jump to label otherwise prints following
                fprintf(output_file, "\tBRzp L_%s_", label); 
                fprintf(output_file, "%d\n", compare_tracker);

                fprintf(output_file, "\n\t;; true\n");
                fprintf(output_file, "\tCONST R2, #1\n");
                fprintf(output_file, "\tJMP L_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                
                fprintf(output_file, "\n\t;; false\n");
                fprintf(output_file, "L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tCONST R2, #0\n");
                
                fprintf(output_file, "\nL_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tSTR R2, R6, #0\n"); 

                compare_tracker ++;
                break;

            case LE :

                fprintf(output_file, "\n\t;; less than or equal\n");
       
                fprintf(output_file, "\tLDR R0, R6, #0\n");
                fprintf(output_file, "\tLDR R1, R6, #1\n");
                fprintf(output_file, "\tADD R6, R6, #1\n");
                fprintf(output_file, "\tCMP R0 R1\n");
                fprintf(output_file, "\tBRp L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);

                fprintf(output_file, "\n\t;; true\n");
                fprintf(output_file, "\tCONST R2, #1\n");
                fprintf(output_file, "\tJMP L_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                
                fprintf(output_file, "\n\t;; false\n");
                fprintf(output_file, "L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tCONST R2, #0\n");
                
                fprintf(output_file, "\nL_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tSTR R2, R6, #0\n");
               
                compare_tracker ++;
                break;    

            case EQ :   

                fprintf(output_file, "\n\t;; equal\n");
       
                fprintf(output_file, "\tLDR R0, R6, #0\n");
                fprintf(output_file, "\tLDR R1, R6, #1\n");
                fprintf(output_file, "\tADD R6, R6, #1\n");
                fprintf(output_file, "\tCMP R0 R1\n");
                fprintf(output_file, "\tBRnp L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);

                fprintf(output_file, "\n\t;; true\n");
                fprintf(output_file, "\tCONST R2, #1\n");
                fprintf(output_file, "\tJMP L_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                
                fprintf(output_file, "\n\t;; false\n");
                fprintf(output_file, "L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tCONST R2, #0\n");
                
                fprintf(output_file, "\nL_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tSTR R2, R6, #0\n");
               
                compare_tracker ++;
                break;       

            case GE :

                fprintf(output_file, "\n\t;; greater than or equal\n");
                fprintf(output_file, "\tLDR R0, R6, #0\n");
                fprintf(output_file, "\tLDR R1, R6, #1\n");
                fprintf(output_file, "\tADD R6, R6, #1\n");
                fprintf(output_file, "\tCMP R0 R1\n");
                fprintf(output_file, "\tBRn L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);

                fprintf(output_file, "\n\t;; true\n");
                fprintf(output_file, "\tCONST R2, #1\n");
                fprintf(output_file, "\tJMP L_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                
                fprintf(output_file, "\n\t;; false\n");
                fprintf(output_file, "L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tCONST R2, #0\n");
               
                fprintf(output_file, "\nL_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tSTR R2, R6, #0\n");

                compare_tracker ++;
                break;

            case GT : 

                fprintf(output_file, "\n\t;; greater than\n");
                
                fprintf(output_file, "\tLDR R0, R6, #0\n");
                fprintf(output_file, "\tLDR R1, R6, #1\n");
                fprintf(output_file, "\tADD R6, R6, #1\n");
                fprintf(output_file, "\tCMP R0 R1\n");
                fprintf(output_file, "\tBRnz L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);

                fprintf(output_file, "\n\t;; true\n");
                fprintf(output_file, "\tCONST R2, #1\n");
                fprintf(output_file, "\tJMP L_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                
                fprintf(output_file, "\n\t;; false\n");
                fprintf(output_file, "L_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tCONST R2, #0\n");
                
                fprintf(output_file, "\nL_end_%s_", label);
                fprintf(output_file, "%d\n", compare_tracker);
                fprintf(output_file, "\tSTR R2, R6, #0\n");

                compare_tracker ++;
                break;

            case IF :

                fprintf(output_file, "\n\t;; if statement\n");

                fprintf(output_file, "\tADD R6, R6, #1\n"); 
                fprintf(output_file, "\tLDR R1, R6, #-1\n");
                fprintf(output_file, "\tBRz ELSE_%d\n", if_tracker);

                push(if_stack, if_tracker);
                push(elseif_stack, if_tracker);

                if_tracker ++;
                break;

             case ELSE :

                fprintf(output_file, "\n\t;; else statement\n");

                fprintf(output_file, "\tJMP ENDIF_%d\n", peek(elseif_stack));
                fprintf(output_file, "\nELSE_%d\n", peek(elseif_stack));

                pop(elseif_stack);

                break;

            case ENDIF : 

                if (peek(elseif_stack)) {
                    if (peek(if_stack) == peek(elseif_stack)) {
                        fprintf(output_file, "\n\t;; else statement\n");
                        fprintf(output_file,"\tELSE_%d\n", pop(elseif_stack));
                    
                    } else {
                        fprintf(output_file, "\n\t;; endif statement\n");
                        fprintf(output_file, "\tENDIF_%d\n", peek(if_stack));
                    }

                } else {
                    fprintf(output_file, "\n\t;; endif statement\n");
                    fprintf(output_file, "\tENDIF_%d\n", peek(if_stack));
                }

                pop(if_stack);

                endif_tracker ++;
                break;

            case DROP :

                fprintf(output_file, "\n\t;; drop\n");
                fprintf(output_file,"\tADD R6 R6 #1\n");

                break;

            case DUP :

                fprintf(output_file, "\n\t;; duplicate\n");

                fprintf(output_file, "\tLDR R1, R6, #0\n");
                fprintf(output_file, "\tADD R6, R6, #-1\n");
                fprintf(output_file, "\tSTR R1, R6, #0\n");

                break;

            case SWAP:

                fprintf(output_file, "\n\t;; swap\n");

                fprintf(output_file, "\tLDR R1, R6, #0\n");
                fprintf(output_file, "\tLDR R2, R6, #1\n");
                fprintf(output_file, "\tSTR R1, R6, #1\n");
                fprintf(output_file, "\tSTR R2, R6, #0\n");

                break;

            case ROT :
                fprintf(output_file, "\n\t;; rotate\n");

                fprintf(output_file, "\tLDR R0, R6, #0\n");
                fprintf(output_file, "\tLDR R1, R6, #1\n");
                fprintf(output_file, "\tLDR R2, R6, #2\n");
                fprintf(output_file, "\tSTR R0, R6, #1\n");
                fprintf(output_file, "\tSTR R1, R6, #2\n");
                fprintf(output_file, "\tSTR R2, R6, #0\n");

                break;

            case ARG :

                arg_pos = theToken -> arg_no + 2;

                fprintf(output_file, "\n\t;; copied arg%d to top of stack\n", theToken -> arg_no);
                
                fprintf(output_file, "\tLDR R1, R5, #%d\n", arg_pos);
                fprintf(output_file, "\tADD R6, R6, #-1\n");
                fprintf(output_file, "\tSTR R1, R6, #0\n");

                break;

            case LITERAL :

                literal_value = (short) theToken -> literal_value;
                const_value = literal_value & 0xFF;
                hiconst_value = (literal_value >> 8) & 0xFF;
         
                fprintf(output_file, "\n\t;; adding literal %d\n", literal_value);

                fprintf(output_file, "\tCONST R1, #%d\n", const_value);
                fprintf(output_file, "\tHICONST R1, #%d\n", hiconst_value);
                fprintf(output_file, "\tADD R6, R6, #-1\n");
                fprintf(output_file, "\tSTR R1, R6, #0\n");

                break;

            case BROKEN_TOKEN :
                printf("BROKEN TOKEN\n"); 
                
                break;

            default : 
                printf("BROKEN TOKEN\n"); 
        }
    }

    if (if_tracker != endif_tracker) {
        printf("ERROR: IF NOT DELIMITED WITH ENDIF\n"); 
    }

    clear(if_stack);
    clear(elseif_stack);

    free(label);
    free(asm_file);
    free(if_stack);
    free(elseif_stack);
    free(theToken);

    fclose(input_file);
    fclose(output_file); 
}