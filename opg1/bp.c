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

	instr->child = NULL;

	return instr;
}

/* Destroy an instruction, destroying its child recursively.
 *
 * Returns 0 if the given instruction is a NULL pointer, returns 1 on
 * success. */
int destroy_instruction(instruction *instr) {
	if (instr == NULL)
		return 0;

	destroy_instruction(instr->child);

	free(instr);

	return 1;
}

int execute_commands(instruction *instr) {
	int input;
	int fd[2];
	pid_t pid;

	/* This variable is used to pass on the output from a previous command to
	 * the current command as input. As the first command executed needs no
	 * input from a previous command, this is set to STDIN. */
	input = STDIN_FILENO;

	/* The last process doesn't need to redirect its output to the next
	 * process, Therefore it is handled seperately, after every other piping
	 * instruction. */
	for (; instr != NULL; instr = instr->child) {

		/* The last command in the list doesn't need a pipe. */
		if (instr->child != NULL) {

			/* Create a pipe from file 1 to file 0. On failure, the process is
			 * terminated. */
			if (pipe(fd) == -1) {
				perror("Error creating pipe");
				return 0;
			}

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

			/* The last command in the list does not need to redirect
			 * output. */
			if (instr->child != NULL) {

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
			}

			/* Execute the command. On failure, the process is terminated. */
			if (execvp(instr->command, instr->arguments) == -1) {
				perror("Error executing command");
				return 0;
			}
		}
		else {

			/* The last command closes input, not fd[1]. */
			if (instr->child == NULL)
				fd[1] = input;

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

			/* Make input file the output from the executed command if the
			 * the instruction is not the latest. */
			if (instr->child != NULL)
				input = fd[0];
		}
	}

	return 1;
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
	char *temp_command, *temp_arguments[1024], *args[100];
	int i, j, x, child, cmd_len;

	temp_instruction = malloc(sizeof(instruction));

	i = 0;

	/* Split up all the command seperated with a pipe character */
	args[i] = strtok(command_line,"|");

	while (args[i++] != NULL) {
		printf("[debug]command:\t %s\n",args[i-1]);
		args[i] = strtok(NULL,"|");
	}

	/* Split up all commands and arguments from each pipe.
	 */
	child = 0;
	for (j = 0; j < (i - 1); j ++) {
		x = 0;

		temp_arguments[x] = strtok(args[j]," ");

		/* Put all arguments in an array */
		while (temp_arguments[x++] != NULL) {
			printf("[debug]arg:\t %s\n", temp_arguments[x-1]);
			temp_arguments[x] = strtok(NULL," ");
		}

		/* Check whether the command was found. */
		if(temp_arguments[0] != NULL) {
			cmd_len = strlen(temp_arguments[0]) + 5;
			temp_command = malloc(sizeof(char) * cmd_len);
			strcpy(temp_command, "/bin/");
			strcat(temp_command, temp_arguments[0]);
		}
		else {
			printf("Bad command given\n");
			return NULL;
		}
		
		/* Check whether the command is the first in a serie of pipes to
		 * create a correct linked list.
		 */
		if(child) {
			temp_instruction->child = create_instruction(temp_command, temp_arguments);
			temp_instruction = temp_instruction->child;

		}
		else {
			temp_instruction = create_instruction(temp_command, temp_arguments);
			first_instruction = temp_instruction;
			child = 1;
		}
	}
	return first_instruction;
}

/* Reads the user's input. Returns a string with the input.
 */
char *read_line(char *dir){
	char buffer[128], *input;
	int buffer_size, size, i;
	printf("%s $ ",dir);
	fgets(buffer, sizeof(buffer), stdin);

	buffer_size = strlen(buffer);

	size 	= (sizeof(char) * buffer_size);
	input 	= malloc(size);

	/* Remove the newline character read by fgets. */
	for(i = 0; i < buffer_size; i++) {
		if(buffer[i] != '\n'){
			input[i] = buffer[i];
		}else {
			input[i] = '\0';
			break;
		}
	}

	return input;
}

int main(int argc, char *argv[]) {
	instruction *first_instruction;
	char *user_input;
	/* Example instruction chain. */

/*	instruction *root, *instr;
	char *args_0[3], *args_1[3], *args_2[3];

	args_0[0] = "ls";
	args_0[1] = "-al";
	args_0[2] = NULL;

	args_1[0] = "grep";
	args_1[1] = "bp";
	args_1[2] = NULL;

	args_2[0] = "grep";
	args_2[1] = "c";
	args_2[2] = NULL;

	root = create_instruction("/bin/ls", args_0);
	instr = (root->child) = create_instruction("/bin/grep", args_1);
	instr->child = create_instruction("/bin/grep", args_2);

	execute_commands(root);

	destroy_instruction(root);*/


	/* Parse command testing. */
	while(1) {
		user_input = read_line("bram@BramLinuxMint ~/Dropbox/Opdrachten/Besturingssystemen/os/opg1");
		printf("[info]executing: %s \n", user_input);

		first_instruction = parse_command(user_input);

		execute_commands(first_instruction);
		destroy_instruction(first_instruction);

		free(user_input);
	}

	return 0;
}
