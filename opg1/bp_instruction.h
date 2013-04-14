/* Shell / bp_instruction.h
 *
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 *
 * All prototypes, constants and structs for work with instructions.
 *
 * The following files are needed to run the shell:
 * bp.{c,h}, bp_handler.{c,h}, bp_instruction.{c,h} and (optional) Makefile. */

#ifndef BP_INSTRUCTION_H
#define BP_INSTRUCTION_H

typedef struct instruction_struct {
	char **command;
	struct instruction_struct *child;
} instruction;

instruction *create_instruction(char **command);
int destroy_instruction(instruction *instr);

#endif
