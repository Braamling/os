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

int execute_commands(instruction **instrs, int n_instrs) {
	int i, input;
	pid_t pid;
	instruction *instr;
	int fd[2];

	/* This variable is used to pass on the output from a previous command to
	 * the current command as input. As the first command executed needs no
	 * input from a previous command, this is set to STDIN. */
	input = STDIN_FILENO;

	/* The last process doesn't need to redirect its output to the next
	 * process, Therefore it is handled seperately, after every other piping
	 * instruction. */
	for (i = 0; i < (n_instrs - 1); i ++) {

		instr = instrs[i];

		/* Create a pipe from file 1 to file 0. On failure, the process is
		 * terminated. */
		if (pipe(fd) == -1) {
			perror("Error creating pipe");
			return 0;
		}

		/* Fork the process. The child links the appropriate file descriptors
		 * to STDIN and STDOUT. On failure, the process is terminated. */
		pid = fork();
		if (pid == -1) {
			perror("Error forkin process");
			return 0;
		}

		/* The child process will duplicate the contents of the input file to
		 * STDIN. The command that will be executed will then use this as its
		 * input. The beginning of the pipe to fd 0 (fd 1), is duplicated to
		 * STDOUT. Fd 0 is then closed, and when the command is executed, its
		 * output will be written to fd 0 in the parent process.
		 * The parent will duplicate this output to the input file. Fd 1 is
		 * then closed, as it won't be necessary anymore. It then continues by
		 * waiting for the child to finish. */
		if (pid == CHILD) {

			/* Duplicate the input file to STDIN. On failure, the process is
			 * terminated. */
			if (dup2(input, STDIN_FILENO) == -1) {
				perror("Error duplicating input file to STDIN");
				return 0;
			}

			/* Duplicate fd 1 to STDOUT. On failure, the process is
			 * terminated. */
			if (dup2(fd[1], STDOUT_FILENO) == -1) {
				perror("Error duplicating fd 1 to STDOUT");
				return 0;
			}

			/* Close fd 0. On failure, the process is terminated. */
			if (close(fd[0]) == -1) {
				perror("Error closing fd 0");
				return 0;
			}

			/* Execute the command. On failure, the process is terminated. */
			if (execvp(instr->command, instr->arguments) == -1) {
				perror("Error executing command");
				return 0;
			}
		}
		else {

			/* Duplicate file 0 to input. On failure, the process is
			 * terminated. */
			// if (dup2(fd[0], input) == -1) {
			// 	perror("Error duplicating input to file 1");
			// 	return 0;
			// }

			/* Close file 1. On failure, the process is terminated. */
			if (close(fd[1]) == -1) {
				perror("Error closing file 1");
				return 0;
			}

			/* Wait for the child to finish. On failure, the process is
			 * terminated. */
			if (wait(NULL) == -1) {
				printf("Error waiting for child process.\n");
				return 0;
			}

			/* Make input file the output from the executed command. */
			input = fd[0];
		}
	}

	/* Execute the last command. Now the output doesn't need to be stored, but
	 * can just go directly to STDOUT. The output from the last command does
	 * need to be duplicated to STDIN though. */
	instr = instrs[n_instrs - 1];

	/* Fork the process. On failure, the process is terminated. */
	pid = fork();
	if (pid == -1) {
		perror("Error forking the process");
		return 0;
	}

	if (pid == CHILD) {

		/* Duplicate the input file to STDIN. On failure, the process is
		 * terminated. */
		if (dup2(input, STDIN_FILENO) == -1) {
			perror("Error duplicating input file to STDIN");
			return 0;
		}

		/* Execute the command. On failure, the process is terminated. */
		if (execvp(instr->command, instr->arguments) == -1) {
			perror("Error executing command");
			return 0;
		}
	}
	else {

		/* Close the input file. On failure, the process is terminated. */
		if (close(input) == -1) {
			perror("Error closing input file");
			return 0;
		}

		/* Wait for the child to finish. On failure, the process is
		 * terminated. */
		if (wait(NULL) == -1) {
			printf("Error waiting for child process.\n");
			return 0;
		}
	}

	return 1;
}

int parse_command(char *command_line) {
	return 0;
}

int main(int argc, char *argv[]) {
	instruction *instrs[2];
	char *args_0[2], *args_1[3];

	args_0[0] = "ls";
	args_0[1] = NULL;
	args_1[0] = "grep";
	args_1[1] = "bp";
	args_1[2] = NULL;

	instrs[0] = create_instruction("/bin/ls", args_0);
	instrs[1] = create_instruction("/bin/grep", args_1);

	execute_commands(instrs, 2);

	destroy_instruction(instrs[0]);
	destroy_instruction(instrs[1]);

	return 0;
}
