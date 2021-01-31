/*
 * LC4.h
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {

	// LC4 memory
	unsigned short int memory[65536];

	unsigned short int reg[8];

	unsigned short int PC;

	unsigned short int PSR;

	unsigned char regFile_WE;
	unsigned char NZP_WE;
	unsigned char DATA_WE;

	unsigned short int reg_input_val;
	unsigned short int NZP_val;
	unsigned short int memory_val;
	unsigned short int memory_address;


}	LC4_state;


void reset (LC4_state *state);

unsigned short int SEXT (unsigned short int num, int bit);

char binary_instr (int num, char instr[]);

void update_state (FILE *output_file, LC4_state *state, int num);

void set_nzp (short val, LC4_state *state);

