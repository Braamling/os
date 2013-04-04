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

int execute_command(unsigned char *command_line) {
	return 0;
}

int parse_command(unsigned char *command_line) {
	char test[39]= "test test | test tst | tet test \n";
	unsigned char *args[10] = {NULL}; 
					 /* This is not really elegant, but
					given the fact that we have a limited
					input-line length, it cannot contain
					more than half as many arguments as
					characters */
	char *temp_begin, *temp_end;
	int i = 0;

	if(args[i++] = strtok(test, "|")){
		while(args[i++] = strtok(NULL, "|"));
	}
	args[i] = strtok(NULL, "\n");

	int j;

	for (j = 0; j < i-1; j++)
	{
		printf("%s \n", strtok(args[j], " "));
		printf("%s \n", strtok(NULL, "\0"));
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

	printf("%s, %s, %s", args[0], args[1], args[2]);
	//strtok(test, '|');

	return 0;
}

int main(int argc, char *argv[]) {


	return 0;
}