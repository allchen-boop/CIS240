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

	FILE *output_file;
	LC4_state *lc4_state;
	int i;

	output_file = fopen (argv[1], "w");
	lc4_state = malloc (sizeof(lc4_state) * MEMORY_SIZE);

	reset (lc4_state); 

	for (i = 2; i < argc; i ++) {
		loader (argv[i], lc4_state -> memory);
	}	
	
	while (lc4_state -> PC != 0x80FF) {
		update_state(output_file, lc4_state, lc4_state -> memory[lc4_state -> PC]);

		// DATA section
		if (lc4_state -> PC >= 0x2000 && lc4_state -> PC <= 0x7FFF) {
			fprintf(output_file,"ILLEGAL ACCESS\n");
			free (lc4_state);
			fclose (output_file);
			exit(1);
			
		}

		// DATA section
		if (lc4_state -> PC >= 0xA000 && lc4_state -> PC <= 0xFFFF) {
			printf("ILLEGAL ACCESS\n");
			free (lc4_state);
			fclose (output_file);
			exit(1);
			
		}

		// OS section
		if (((lc4_state -> PSR) >> 15) == 0) {
			if (lc4_state -> PC >= 0x8000 && lc4_state -> PC >= 0xFFFF) {
				printf("ILLEGAL ACCESS\n");
				free (lc4_state);
				fclose (output_file);
				exit(1);
			}
		}
	}

	free (lc4_state);
	fclose (output_file);
}
