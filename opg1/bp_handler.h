/* Shell / bp_handler.h
 *
 * bp_handler contains all prototypes, constants and structs for the command 
 * handling functions.
 *
 * The following files are needed to run the shell:
 * bp.h, bp.c, bp_handler.c, bp_handler.h and (optional) Makefile.
 * 
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 */

#ifndef HANDLER
#define HANDLER

#define CHILD 0
#define MAX_INSTRUCTIONS 10
#define TERMINATE 1
#define ALLOW_CD 1
#define DISALLOW_CD 0

struct sigaction old_action;
int child_process;

typedef struct instruction_struct {
	char **command;
	struct instruction_struct *child;
} instruction;

instruction *create_instruction(char **command);
int destroy_instruction(instruction *instr);
int execute_commands(instruction *instr, int input_fd);
instruction *parse_command(char *command_line);
int cd(char *cmd);
char *trim_start(char *string);
int execute_file(char *command);
int run_line(char *line, int may_cd);

#endif
