/* Shell / bp_handler.h
 *
 * bp_handler contains all prototypes and constants for the 
 * functions.
 *
 * The following files are needed to run the shell:
 * bp.h, bp.c, bp_handler.c, bp_handler.h and (optional) Makefile.
 * 
 * Authors: 
 * -Bas van den Heuvel
 * -Bram van den Akker
 */

void print_help();
void sigint_handler(int sig_no);
char *build_prompt();
