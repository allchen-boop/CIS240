/*
 * parser.c : parses terminal input for the output file and the obj files
 */

#include "LC4.h"
#include "loader.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 65536

int main (int argc, char **argv) {

	int i;
	unsigned short address;
	unsigned short hex_value;
	
	FILE *output_file;
	LC4_state *lc4_memory;
	LC4_state *lc4_instruction;

	output_file = fopen (argv[1], "w");
	lc4_memory = malloc (sizeof(lc4_memory) * MEMORY_SIZE);
	lc4_instruction = malloc (sizeof(lc4_instruction) * MEMORY_SIZE);

	reset (lc4_memory);

	for (i = 2; i < argc; i ++) {
		loader (argv[i], lc4_memory -> memory);
	}

	i = 0;

	while (i < MEMORY_SIZE) {
		address = i;
		hex_value = lc4_memory -> memory[i];

		if ((i < 0xA000 && i >= 0x8000) || i < 0x2000) { 
			
			// nothing is printed when value is 0
			if (hex_value != 0) {
				decode (hex_value, lc4_instruction-> instruction);
				fprintf (output_file, "%04X : %04X -> %s\n", address, hex_value, lc4_instruction-> instruction);
			}

		} else if (hex_value != 0) {
				fprintf (output_file, "%04X : %04X\n", address, hex_value);
			}
		
		i ++;
	}
	
	free (lc4_memory);
	free (lc4_instruction);
	fclose (output_file);
}
