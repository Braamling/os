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
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "bp.h"

struct sigaction old_action;
struct sigaction prompt_action;
struct sigaction piping_action;

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
	int fd[2], status;
	pid_t pid, wait_pid;

	sigaction(SIGINT, &piping_action, &old_action);

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

		/* Reactivate the ^C termination sigaction sigint. */
		sigaction(SIGINT, &old_action, NULL);

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

			/* The command failed to execute. Terminate the child. */
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

		do {
			wait_pid = wait(&status);
		} while ((wait_pid == -1) && (errno == EINTR));

		sigaction(SIGINT, &prompt_action, &old_action);

		if (wait_pid == -1) {
			perror("Error waiting for child process");
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
		if(temp_arguments[0] == NULL){
			return NULL;
		}

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

	printf("%s $ ",dir);
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

/* Change directory. Input is the command, including 'cd'. */
int cd(char *cmd) {
	char *path;

	path = &cmd[3];
	path = trim_start(path);

	if (chdir(path) == -1) {
		perror("Error changing directory");
		return -1;
	}

	return 0;
}

/* Remove all spaces and tabs from the beginning of the string. */
char *trim_start(char *string) {
	while (isblank(string[0]))
		string = &string[1];

	return string;
}

/* Execute a file line by line. */
int execute_file(char *command) {
	FILE *fp;
	ssize_t read;
	size_t len;
	char *path, *line;
	int run_result;

	/* As executing a file is done by giving the command '. <filename>',
	 * extract the filename, by creating a pointer to that start of the
	 * filename in the command string. */
	path = &command[2];

	run_result = 0;

	fp = fopen(path, "r");
	if (fp == NULL)
		return -1;

	len = 0;
	line = NULL;

	while ((read = getline(&line, &len, fp)) != -1) {

		/* Remove the newline char from the line. */
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		run_result = run_line(line, DISALLOW_CD);

		/* If the file called 'exit', stop executing the file, but don't stop
		 * the shell. */
		if (run_result == TERMINATE) {
			run_result = 0;
			break;
		}
		else if (run_result == -1) {

			/* An instruction failed, break off execution. */
			break;
		}
	}

	free(line);

	return run_result;
}

/* Runs a line of commands.
 *
 * Returns 0 on success, -1 if something went wrong, and 1 when exit has been
 * called. */
int run_line(char *line, int may_cd) {
	instruction *instr;
	char *possible_cd;
	int do_cd;

	if (line == NULL) {
		/* End of file, terminate. */
		printf("\n");
		return TERMINATE;
	}

	possible_cd = NULL;

	line = trim_start(line);

	// printf("[debug] Line:%s\n", line);

	/* Extract the first three characters from the line, so we can check if
	 * a 'cd ' command was given. */
	possible_cd = malloc(4);
	memcpy(possible_cd, line, 3);
	possible_cd[3] = '\0';
	do_cd = strcmp(possible_cd, "cd ") == 0;
	free(possible_cd);

	if (strcmp(line, "") == 0) {

		/* No command given, do nothing. */
		return 0;
	}
	else if (strcmp(line, "exit") == 0) {

		/* Tell the program to stop. */
		return TERMINATE;
	}
	else if (line[0] == '.') {

		/* Run commands from a file. */
		return execute_file(line);
	}
	else if (do_cd) {

		/* Check if changing dir is allowed. */
		if (may_cd == DISALLOW_CD) {

			/* Just do nothing. */
			return 0;
		}
		
		/* Change directory to the given path. */
		return cd(line);
	}
	else if (line[0] == '/') {

		/* If a '/' occurs in a command, the user could run mallicious code.
		 * This is not allowed. */
		printf("[warning] Cannot execute binairy outside PATH.\n");
		return -1;
	}
	else {

		/* Just parse the line and execute it. */

		instr = parse_command(line);

		if (instr == NULL) {
			return -1;
		}

		// printf("[debug]first instruction:\t%s, %s\n",
		//	instr->command[0], instr->command);

		execute_commands(instr, -1);
		destroy_instruction(instr);
		
		return 0;
	}
}

char *build_prompt() {
	char *cwd, *prompt;

	cwd = get_current_dir_name();

	prompt = malloc(strlen(cwd) + 4);
	prompt = strcpy(prompt, cwd);
	prompt = strcat(prompt, " $ ");

	free(cwd);

	return prompt;
}

void prompt_sigint_handler(int sig_no) {
	char *prompt;

	prompt = build_prompt();

	printf("\n%s", prompt);

	free(prompt);
}

void piping_sigint_handler(int sig_no) {
	printf("\n");
}

int main(int argc, char *argv[]) {
	char *prompt, *user_input;
	int running, run_result;

	running = 1;

	/* Create a new sigaction for ^C interups. */
	memset(&prompt_action, 0, sizeof(prompt_action));
	prompt_action.sa_handler = &prompt_sigint_handler;

	/* Prepare a sigaction for when a piping string of commands is interrupted
	 * by the keyboard (^C). */
	memset(&piping_action, 0, sizeof(piping_action));
	piping_action.sa_handler = &piping_sigint_handler;

	/* Redirect sigaction to escape keyboard interups. */
	sigaction(SIGINT, &prompt_action, &old_action);

	while (running) {

		prompt = build_prompt();

		user_input = readline(prompt);

		add_history(user_input);
		
		run_result = run_line(user_input, ALLOW_CD);

		if (run_result == -1) {
			printf("Error running commands.\n");
		}
		else if (run_result == TERMINATE)
			running = 0;

		free(prompt);
		free(user_input);

		/* Redirect sigaction to escape ^C interups */
		sigaction(SIGINT, &prompt_action, &old_action);
	}

	return 0;
}
