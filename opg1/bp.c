/* Shell / bp.c
 *
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 *
 * The following program is a linux command shell. 
 * Bp has the following features:
 * - Parsing commands to allow the use of arguments
 * - Piping command output to other commands.
 * - Using the readline/history package to easily reuse used commands
 * - Catching keyboard sigint calls and redirecting them in the parent proces
 *
 * Readline/history.h might not work in the first place. In this case please
 * download readline by using the follow command in the terminal 
 * (debian based systems only):
 * 
 * sudo apt-get install libreadline6 libreadline6-dev
 * 
 * The following files are needed to run the shell:
 * bp.{c,h}, bp_handler.{c,h}, bp_instruction.{c,h} and (optional) Makefile.
 *
 * The shell can be exited by typing 'exit' when prompted for input.
 * ^C will not exit the shell therefore in case of an emergency ^D can be used 
 * to close the shell. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "bp.h"
#include "bp_handler.h"

/* Function that is called after catching a sigint interupt. 
 */
void sigint_handler(int sig_no){
	char *prompt;

	printf("\n");

	if (!child_process) {

		prompt = build_prompt();

		printf("%s", prompt);

		free(prompt);		
	}
}

/* Function for building a prompt string consisting of the path and a prompt 
 * character. Returns the prompt message as a char pointer. */
char *build_prompt() {
	char *cwd, *prompt;

	cwd = get_current_dir_name();

	prompt = malloc(strlen(cwd) + 4);
	prompt = strcpy(prompt, cwd);
	prompt = strcat(prompt, " $ ");

	free(cwd);

	return prompt;
}

int main(int argc, char *argv[]) {
	char *prompt, *user_input;
	int running, run_result;

	running = 1;

	/* Create a new sigaction for ^C interups */
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = &sigint_handler;

	/* Redirect sigaction to escape keyboard interups. */
	sigaction(SIGINT, &action, &old_action);

	while (running) {

		prompt = build_prompt();

		user_input = readline(prompt);

		add_history(user_input);
		
		run_result = run_line(user_input, ALLOW_CD);
		child_process = 0;

		if (run_result == -1) {
			printf("Error running commands.\n");
		}
		else if (run_result == TERMINATE)
			running = 0;

		free(prompt);
		free(user_input);
	}

	return 0;
}
