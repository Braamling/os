/* Shell / bp_instruction.c
 *
 * Authors:
 * -Bas van den Heuvel
 * -Bram van den Akker
 *
 * Functionality to work with instructions (linked list).
 *
 * The following files are needed to run the shell:
 * bp.{c,h}, bp_handler.{c,h}, bp_instruction.{c,h} and (optional) Makefile. */

#include <stdlib.h>
#include <unistd.h>

#include "bp_instruction.h"

/* Construct an instruction.
 *
 * Returns NULL on failure, a pointer to the newly created instruction on
 * success. */
instruction *create_instruction(char **command) {
	instruction *instr;

	instr = malloc(sizeof(instruction));
	if (instr == NULL)
		return NULL;

	instr->command = command;

	instr->child = NULL;

	return instr;
}

/* Destroy an instruction, destroying its child recursively.
 *
 * Returns -1 if the given instruction is a NULL pointer, returns 0 on
 * success. */
int destroy_instruction(instruction *instr) {
	if (instr == NULL)
		return 0;

	destroy_instruction(instr->child);

	free(instr->command);
	free(instr);

	return 1;
}
