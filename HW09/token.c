/*
 * token.c : read in tokens from j file to generate asm code
 */

#include "token.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 250

int valid_arg(char *str) {
	if (strlen(str) > 3 && strlen(str) < 6) {
		if (isdigit(str[3])) {
			if (atoi(&str[3]) > 0 && atoi(&str[3]) < 21) {
				return 0;
			}
		}
	}
	return 1;
}

int valid_hex(char *str) {
	int i;
	if (strlen(str) < 3 || strlen(str) > 6) {
		return 1;
	}
	
	for (i = 2; i < strlen(str); i ++) {
		if (!isxdigit(str[i])) {
			return 1;
		}
	}
	return 0;
}

int valid_decimal(char *str) {
	int i;
	if (str[0] == '-') {
		for (i = 1; i < strlen(str); i ++) {
  			if (!isdigit(str[i])) {
		  		return 1;
		  	}
		}
	} else {
		for (i = 0; i < strlen(str); i ++) {
  			if (!isdigit(str[i])) {
		  		return 1;
		  	}
		}

	}
	return 0;
}

int valid_name(char *str) {
	int i;
	if (!isalpha(str[0])) {
		return 1;
	}

	for (i = 1; i < strlen(str); i++) {
		if (!isalnum(str[i])) {
			if (str[i] != '_') {
				return 1;
			}
		}
	}
	return 0;
}

int read_token (token *theToken, FILE *theFile) {

	int i ;  
  	char c = getc(theFile) ;

  	//reading token
  	while (c != EOF) {
  		// space
		if (isspace(c)) {
			while (isspace(c)) {
				c = getc(theFile) ; 
    		}
    	// comment
    	} else if (c == ';') {
    		c = getc(theFile) ;

    		while (c != '\n') {
    			c = getc(theFile); 
    		}
      	// string
   		} else {
    		i = 0;
    		while (i < MAX_TOKEN_LENGTH) {
    			if (isspace(c) || c == EOF) {
					theToken -> str[i] = '\0';
    				break; 
    			}
				
				theToken -> str[i] = c ; 
    			c = getc(theFile);
    			i ++;
    		}

    		break; 
    	}  
  	}

  	if (!strcmp (theToken -> str, "defun")) {
	    theToken -> type = DEFUN;
	    return 0;

	} else if (!strcmp (theToken -> str, "return")) {
		theToken -> type = RETURN;
    	return 0 ; 
  
  	} else if (!strcmp (theToken -> str, "+")) {
	    theToken -> type = PLUS;
	    return 0; 
  
	} else if (!strcmp (theToken -> str, "-")) {
	    theToken -> type = MINUS;
	    return 0;

	} else if (!strcmp (theToken -> str, "*")) {
	    theToken -> type = MUL;
	    return 0; 

	} else if (!strcmp (theToken -> str, "/")) {
	    theToken -> type = DIV;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "%")) {
	    theToken -> type = MOD;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "and")) {
	    theToken -> type = AND;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "or")) {
	    theToken -> type = OR;
	    return 0; 
	
	} else if (!strcmp (theToken -> str, "not")) {
	    theToken -> type = NOT;
	    return 0; 
	
	} else if (!strcmp (theToken -> str, "lt")) {
	    theToken -> type = LT;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "le")) {
	    theToken -> type = LE;
	    return 0; 
  
  	} else if (!strcmp (theToken -> str, "eq")) {
	    theToken -> type = EQ;
	    return 0; 

	 } else if (!strcmp (theToken -> str, "ge")) {
	    theToken -> type = GE;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "gt")) {
	    theToken -> type = GT;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "if")) {
	    theToken -> type = IF;
	    return 0; 
  	
  	} else if (!strcmp (theToken -> str, "else")) {
	    theToken -> type = ELSE;
	    return 0 ; 

	} else if (!strcmp (theToken -> str, "endif")) {
	    theToken -> type = ENDIF;
	    return 0 ; 
  
	} else if (!strcmp (theToken -> str, "drop")) {
	    theToken -> type = DROP;
	    return 0; 
  
	} else if (!strcmp (theToken -> str, "dup")) {
	    theToken -> type = DUP;
	    return 0; 
	
	} else if (!strcmp (theToken -> str, "swap")) {
	    theToken -> type = SWAP;
	    return 0; 
	
	} else if (!strcmp (theToken -> str, "rot")) {
	    theToken -> type = ROT;
	    return 0;

	// arg
	} else if (theToken -> str[0] == 'a' &&
			   theToken -> str[1] == 'r' && 
			   theToken -> str[2] == 'g') {

		if (!valid_arg(theToken -> str)) {
			theToken -> type = ARG; 
			sscanf(theToken -> str, "arg%d", &(theToken -> arg_no));
			return 0;
		}

	//check hex literal
	} else if (theToken -> str[0] == '0' && theToken -> str[1] == 'x') {
		if (!valid_hex(theToken -> str)) {
			theToken -> type = LITERAL ; 
		  	sscanf(theToken -> str, "%x", &theToken -> literal_value) ;
			return 0;
		}

	//check decimal literal
  	} else if (theToken -> str[0] == '-' || isdigit(theToken -> str[0])) {
  		if (!valid_decimal(theToken -> str)) {
  			theToken -> type = LITERAL;
  			theToken -> literal_value = atoi(theToken -> str);
			return 0;
		}

	// ident
	} else {
		if (!valid_name(theToken -> str)) {
		    theToken -> type = IDENT ;
		    return 0;
		}
	}

	theToken -> type = BROKEN_TOKEN; 
	return 1;
}
