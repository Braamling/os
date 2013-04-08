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
#include <sys/types.h>
#include <sys/wait.h>

#include "bp.h"

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

int execute_commands(instruction *instr, int input_fd) {
	int fd[2];
	pid_t pid;

	/* The last command in the list outputs to STDOUT, so redirecting output is
	 * only necessary for instructions before the last one. */
	if (instr->child != NULL) {

		if (pipe(fd) == -1) {
			perror("Error creating pipe");
			return -1;
		}
	}

	pid = fork();

	if (pid == -1) {
		perror("Error forking process");
		return -1;
	}

	if (pid == CHILD) {

		/* The first command takes input from STDIN, so when this command is
		 * executed, input_fd is set to NULL and no input redirecting is
		 * necessary. */
		if (input_fd != -1) {

			if (dup2(input_fd, STDIN_FILENO) == -1) {
				perror("Error duplicating input to STDIN");
				return -1;
			}
		}

		/* Again, redirecting output is only necessary for instructions before
		 * the last one. */
		if (instr->child != NULL) {

			if (dup2(fd[1], STDOUT_FILENO) == -1) {
				perror("Error duplicating file 1 to STDOUT");
				return -1;
			}

			if (close(fd[0]) == -1) {
				perror("Error closing file 0");
				return -1;
			}
		}

		if (execvp(instr->command[0], instr->command) == -1) {
			perror("Error executing command");
			exit(0);
			return -1;
		}

		return 0;
	}
	else {

		/* The last command didn't make a pipe, but a file from previous
		 * commands could still be open, close this. Any other commands just
		 * close file 1. */
		if (instr->child == NULL)
			fd[1] = input_fd;

		/* Only try to close if this is actually set (i.e. single command). */
		if ((fd[1] != -1) && (close(fd[1]) == -1)) {
			perror("Error closing file 1 (or input)");
			return -1;
		}

		if (wait(NULL) == -1) {
			printf("Error waiting for child process.\n");
			return -1;
		}

		/* If there are any, recursively execute next commands. */
		if (instr->child != NULL)
			return execute_commands(instr->child, fd[0]);
		else
			return 0;
	}
}

/* Parse command uses the user input and create seprate 
 * commands and arguments. 
 * The user input as an char pointer has to be given as an argument.
 * The function will return the first instruction of a linked list with
 * instruction structs. 
 * When a bad command format is found a NULL will be returned.
 */
instruction *parse_command(char *command_line) {
	instruction *temp_instruction, *first_instruction;
	char **temp_arguments, *args[100];
	int i, j, x, child;

	// temp_instruction = malloc(sizeof(instruction));
	
	i = 0;

	/* Split up all the command seperated with a pipe character */
	args[i] = strtok(command_line, "|");

	while (args[i++] != NULL) {
		// printf("[debug]command:\t %s\n",args[i-1]);
		args[i] = strtok(NULL, "|");
	}

	/* Split up all commands and arguments from each pipe. */
	child = 0;
	for (j = 0; j < (i - 1); j ++) {
		x = 0;

		temp_arguments = malloc(sizeof(char *) * 1024);

		temp_arguments[x] = strtok(args[j], " ");

		// if (child)
		// 	printf("[debug]first arg begin loop: \t %s\n",
		// 			first_instruction->command[0]);

		/* Put all arguments in an array */
		while (temp_arguments[x ++] != NULL) {
			// printf("[debug]arg:\t %s\n", temp_arguments[x - 1]);
			temp_arguments[x] = strtok(NULL, " ");
		}

		/* Add NULL pointer for exec. */
		temp_arguments[x] = NULL;

		/* Check whether the command was found. */
		if (temp_arguments[0] == NULL) {
			printf("Bad command given\n");
			return NULL;
		}

		/* Check whether the command is the first in a serie of pipes to
		 * create a correct linked list. */
		if (child) {
			temp_instruction->child = create_instruction(temp_arguments);
			temp_instruction = temp_instruction->child;
		}
		else {
			temp_instruction = create_instruction(temp_arguments);
			first_instruction = temp_instruction;

			child = 1;
			// printf("[debug]first arg after copy: \t %s\n",
			// 		first_instruction->command[0]);
		}
	}
	

	return first_instruction;
}

/* Reads the user's input. Returns a string with the input. */
char *read_line(char *dir){
	char buffer[128], *input;
	int buffer_size, size, i;
	printf("%s$ ",dir);
	fgets(buffer, sizeof(buffer), stdin);

	buffer_size = strlen(buffer);

	size 	= (sizeof(char) * buffer_size);
	input 	= malloc(size);

	/* Remove the newline character read by fgets. */
	for (i = 0; i < buffer_size; i++) {
		if (buffer[i] != '\n') {
			input[i] = buffer[i];
		}
		else {
			input[i] = '\0';
			break;
		}
	}

	return input;
}

/* Runs a line of commands.
 *
 * Returns 0 on success, -1 if something went wrong, and 1 when exit has been
 * called. */
int run_line(char *line) {
	instruction *instr;

	/* Remove all spaces and tabs from the beginning of the line. */
	while ((line[0] == ' ') || (line[0] == '\t'))
		line = &line[1];

	// printf("[debug] Line:%s\n", line);

	if (strcmp(line, "") == 0) {

		/* No command given, do nothing. */
		return 0;
	}
	else if (strcmp(line, "exit") == 0) {

		/* Tell the program to stop. */
		return 1;

	}
	else if (line[0] == '.')
		printf("execute file\n");
	else if (strchr(line, '/') != NULL) {

		/* If a '/' occurs in a command, the user could run mallicious code.
		 * This is not allowed. */
		printf("[warning] Cannot execute binairy outside PATH.");
		return -1;
	}
	else if (strcmp(line, "cd") == 0) {
		// Do nothing.
		return 0;
	}
	else {
		instr = parse_command(line);

		// printf("[debug]first instruction:\t%s, %s\n",
		//	instr->command[0], instr->command);

		execute_commands(instr, -1);
		destroy_instruction(instr);
		
		return 0;
	}

}

int main(int argc, char *argv[]) {
	char *user_input;
	int running, run_result;

	running = 1;

	while (running) {
		user_input = read_line("");

		// printf("[info]executing: %s \n", user_input);
		
		run_result = run_line(user_input);
		if (run_result == -1) {
			printf("Error running commands.\n");
		}
		else if (run_result == 1)
			running = 0;

		free(user_input);
	}

	return 0;
}
