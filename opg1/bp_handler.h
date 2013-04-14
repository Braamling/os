/* Shell / bp_handler.h
 *
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 *
 * All prototypes, constants and structs for the command handling functions.
 *
 * The following files are needed to run the shell:
 * bp.{c,h}, bp_handler.{c,h}, bp_instruction.{c,h} and (optional) Makefile. */

#ifndef BP_HANDLER_H
#define BP_HANDLER_H

#define CHILD 0
#define MAX_INSTRUCTIONS 10
#define TERMINATE 1
#define ALLOW_CD 1
#define DISALLOW_CD 0

struct sigaction old_action;
int child_process;

struct instruction_struct;

int execute_commands(struct instruction_struct *instr, int input_fd);
struct instruction_struct *parse_command(char *command_line);
int cd(char *cmd);
char *trim_start(char *string);
int execute_file(char *command);
int run_line(char *line, int may_cd);

#endif
