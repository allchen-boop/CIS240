/*
 * LC4.h
 */

typedef struct {

	// LC4 memory
	unsigned short int memory[65536];

	// decoded instruction from memory
	char instruction [50];

}	LC4_state;

void reset (LC4_state *state);
unsigned short int SEXT (unsigned short int num, int bit);
char *decode (unsigned short int num, char *instruction);
