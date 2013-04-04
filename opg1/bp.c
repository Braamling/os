/* Shell / bp.c
 *
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "bp.h"

/* Construct an instruction.
 *
 * Returns NULL on failure, a pointer to the newly created instruction on
 * success. */
instruction *create_instruction(char *command, char *arguments) {
	instruction *instr;

	instr = malloc(sizeof(instruction));
	if (instr == NULL)
		return NULL;

	instr->command = command;
	instr->arguments = arguments;

	return instr;
}

/* Destroy an instruction.
 *
 * Returns 0 if the given instruction is a NULL pointer, returns 1 on
 * success. */
int destroy_instruction(instruction *instr) {
	if (instr == NULL)
		return 0;

	free(instr);

	return 1;
}

int execute_commands(char **commands, char *arguments) {
	return 0;
}

int parse_command(char *command_line) {
	return 0;
}

int main(int argc, char *argv[]) {
	return 0;
}
