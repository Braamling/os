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
instruction *create_instruction(char *command, char **arguments) {
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

instruction **parse_command(char *command_line) {
	instruction **instructions;
	int MAX_INSTRUCTIONS = 10;

	instructions = malloc(sizeof(instruction) * MAX_INSTRUCTIONS);

	char test[39]= "111 b111 b112 | 222 221 222 | 333 331 332 \n ";
	char *args[10] = {NULL}; 
	char *temp_command, *temp_argument, **temp_arguments[1024];
	int i = 0;

	if(args[i++] = strtok(test, "|")){
		while(args[i++] = strtok(NULL, "|"));
	}
	args[i] = strtok(NULL, "\n");

	int j;

	for (j = 0; j < i-1; j++) {
		int x = 0;
		temp_command = strtok(args[j], " ");
		temp_argument = strtok(NULL, "\0");
		if(temp_arguments[x++] = strtok(temp_argument, " ")) {
			while(temp_arguments[x++] = strtok(NULL, " "));
		}
		temp_arguments[x] = strtok(NULL, "\0");
		temp_command = strdup(temp_command);


		/* concat /bin/ */
		instructions[j] = create_instruction(temp_command, temp_arguments);
	}

	//temp_end = strtok(test, "|");
	//temp_begin = strtok(NULL, "|");

/*	while(temp = strtok(NULL, "|")){
		args[i++] = strtok(temp, " ");
		args[i++] = strtok(NULL, " ");
	}
	if(temp = strtok(NULL, "\n")){
		args[i++] = strtok(temp, " ");
		args[i++] = strtok(NULL, " ");
	}*/

	printf("%s, %s, %s\n", instructions[0]->arguments[1], instructions[1]->command, instructions[2]->command);
	//strtok(test, '|');

	return instructions;
}

int main(int argc, char *argv[]) {
	parse_command("test");

	return 0;
}
