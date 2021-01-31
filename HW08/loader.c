/*
 * loader.c : reads in the binary and loads into LC4 memory at appropriate address
 */

#include "LC4.h"
#include "loader.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// big endian to little endian
unsigned short int endian (unsigned short int num) {
	return (num & 0xFF00) >> 8 | (num & 0x00FF) << 8;
}


// load binary file into array representing LC4 memory
unsigned short int *loader (char *filename, unsigned short int *lc4_memory) {
	
	unsigned short int instruction;	 
	unsigned short int header;
	unsigned short int address;
	unsigned short int line;
	unsigned short int file;

	unsigned short int num;
	unsigned short int size;

	// open file for binary reading
	FILE *obj_file = fopen (filename, "rb");

	if (obj_file == NULL) {
		printf("UNABLE TO OPEN BINARY FILE\n");
		exit(1);
	}

	// read each 4 bit sequence
	while (fread (&num, sizeof(unsigned short int), 1, obj_file) == 1) {

		// convert endian
		header = endian (num);

		// if header for .CODE or .DATA has been reached
		if (header == 0xCADE || header == 0xDADA) {

			// read next word as address
			fread (&num, sizeof(unsigned short int), 1, obj_file);
			address = endian(num);

			// read following word as number of instructions/data values
			fread (&num, sizeof(unsigned short int), 1, obj_file);
			size = endian(num);

			while (size > 0) {
				fread (&num, sizeof(unsigned short int), 1, obj_file);
				instruction = endian(num);
				lc4_memory[address] = instruction;
				address ++;
				size --;
			}

		// if we have reached header for SYMBOL or file name
		} else if (header == 0xC3B7 || header == 0xF17E) {

			fread(&num, sizeof(unsigned short int), 1, obj_file);
			address = endian(num);

			fread(&num, sizeof(unsigned short int), 1, obj_file);
			size = endian(num);

			// reads and ignore symbol/file name 
			while (size > 0) {
				fread(&num, 1, 1, obj_file);
				size --;
			}

		// skips line number
		} else if (header == 0x715E) {
			fread(&num, sizeof(unsigned short int), 1, obj_file);
			address = endian(num);

			fread(&num, sizeof(unsigned short int), 1, obj_file);
			line = endian(num);

			fread(&num, sizeof(unsigned short int), 1, obj_file);
			file = endian(num);

		}
	}

	// close file
	fclose(obj_file);
	
	return lc4_memory;
}