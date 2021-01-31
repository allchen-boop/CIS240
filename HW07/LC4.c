/*
 * LC4.c: decodes instructions stored in memory to their corresponding mnemonic
 */

#include "LC4.h"
#include "loader.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 65536

// reset LC4 memory
void reset (LC4_state *state) {
	int i;

	for (i = 0; i < MEMORY_SIZE; i ++) {
		state -> memory[i] = 0;
	}
}

// sign extending
unsigned short int SEXT (unsigned short int instr, int bit) {
  unsigned short int num = 0xFFFF;
  unsigned short int msb = instr >> (bit - 1);
  unsigned short int extended = instr;

  if (msb == 1) {
  	num = num >> bit;
    num = num << bit;
    extended = num | instr;
  }

  return extended;
}

// decodes binary instructions to their corresponding string mnemonic
char *decode (unsigned short int num, char *instruction) {	

	unsigned int opcode = num >> 12;
	unsigned int instr_11_9 = (num >> 9) & 0x7;
	unsigned int instr_8_7 = (num >> 7) & 0x3;
	unsigned int instr_8_6 = (num >> 6) & 0x7;
	unsigned int instr_5_4 = (num >> 4) & 0x3;
	unsigned int instr_5_3 = (num >> 3) & 0x7;
	unsigned int instr_2_0 = num & 0x7;
	unsigned int instr_11 = (num >> 11) & 0x1;
	unsigned int instr_5 = (num >> 5) & 0x1;

	unsigned int UIMM4 = num & 0xF;
	unsigned int UIMM8 = num & 0xFF;
	unsigned int UIMM7 = num & 0x7F;

	signed short int IMM11 = (short) SEXT (num & 0x7FF, 11);
	signed short int IMM9 = (short) SEXT (num & 0x1FF, 9);
	signed short int IMM7 = (short) SEXT (UIMM7, 7);
	signed short int IMM6 = (short) SEXT (num & 0x3F, 6);
	signed short int IMM5 = (short) SEXT (num & 0x1F, 5);

	switch (opcode) {
		
		case 0:
			switch (instr_11_9) {

				case 0: 
					sprintf(instruction, "%s", "NOP");
					break;

				case 1:
					sprintf(instruction, "BRp #%d", IMM9);
					break;

				case 2:
					sprintf(instruction, "BRz #%d", IMM9);
					break;

				case 3:
					sprintf(instruction, "BRzp #%d", IMM9);
					break;
				
				case 4:
					sprintf(instruction, "BRn #%d", IMM9);
					break;
					
				case 5:
					sprintf(instruction, "BRnp #%d", IMM9);
					break;
				
				case 6:
					sprintf(instruction, "BRnz #%d", IMM9);
					break;

				case 7:
					sprintf(instruction, "BRnzp #%d", IMM9);
					break;

				default:
					sprintf(instruction, "%s", "INVALID INSTRUCTION");
			}

			break;

	case 1:
		if (instr_5_3 == 0) { 
			sprintf(instruction, "ADD R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5_3 == 1) { 
			sprintf(instruction, "MUL R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5_3 == 2) { 
			sprintf(instruction, "SUB R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5_3 == 3) { 
			sprintf(instruction, "DIV R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5 == 1) { 
			sprintf(instruction, "ADD R%d, R%d, #%d", instr_11_9, instr_8_6, IMM5);
		
		} else {
			sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}

		break;

	case 5:		
		if (instr_5_3 == 0) { 
			sprintf(instruction, "AND R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5_3 == 1) {
			sprintf(instruction, "NOT R%d, R%d", instr_11_9, instr_8_6);
		
		} else if (instr_5_3 == 2) { 
			sprintf(instruction, "OR R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
		
		} else if (instr_5_3 == 3) { 
			sprintf(instruction, "XOR R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);

		} else if (instr_5 == 1) { 
			sprintf(instruction, "AND R%d, R%d, #%d", instr_11_9, instr_8_6, IMM5);
		
		} else { 
			sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}

		break;

	case 6:
		sprintf(instruction, "LDR R%d, R%d, #%d", instr_11_9, instr_8_6, IMM6);
		break;
	
	case 7:
		sprintf(instruction, "STR R%d, R%d, #%d", instr_11_9, instr_8_6, IMM6);
		break;
	
	case 9:
		sprintf(instruction, "CONST R%d, #%d", instr_11_9, IMM9);
		break;
	
	case 13:
		sprintf(instruction, "HICONST R%d, #%d", instr_11_9, UIMM8);
		break;
	
	case 2:	
		switch (instr_8_7) {
			case 0:
				sprintf(instruction, "CMP R%d, R%d", instr_11_9, instr_2_0);
				break;

			case 1: 
				sprintf(instruction, "CMPU R%d, R%d", instr_11_9, instr_2_0);
				break;

			case 2:
				sprintf(instruction, "CMPI R%d, #%d", instr_11_9, IMM7);
				break;

			case 3:
				sprintf(instruction, "CMPIU R%d, #%d", instr_11_9, UIMM7);
				break;

			default:
				sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}

		break;

	case 10:
		switch (instr_5_4) {
			case 0:
				sprintf(instruction, "SLL R%d, R%d, #%d", instr_11_9, instr_8_6, UIMM4);
				break;

			case 1: 
				sprintf(instruction, "SRA R%d, R%d, #%d", instr_11_9, instr_8_6, UIMM4);
				break;

			case 2:
				sprintf(instruction, "SRL R%d, R%d, #%d", instr_11_9, instr_8_6, UIMM4);
				break;

			case 3:
				sprintf(instruction, "MOD R%d, R%d, R%d", instr_11_9, instr_8_6, instr_2_0);
				break;

			default:
				sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}

		break;
	
	case 4:
		switch(instr_11) {
			case 0:
				sprintf(instruction, "JSRR R%d", instr_8_6);
				break;

			case 1:
				sprintf(instruction, "JSR #%d", IMM11);
				break;

			default:
				sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}
		
		break;

	case 12:
		switch(instr_11) {
			case 0:
				sprintf(instruction, "JMPR R%d", instr_8_6);
				break;

			case 1:
				sprintf(instruction, "JMP #%d", IMM11);
				break;

			default:
				sprintf(instruction, "%s", "INVALID INSTRUCTION");
		}
		
		break;

	case 15:
		sprintf(instruction, "TRAP #%d", UIMM8);
		break;

	case 8:
		sprintf(instruction, "%s", "RTI");
		break;

	default:
		sprintf(instruction, "%s", "INVALID INSTRUCTION");
	}

	return instruction;
}