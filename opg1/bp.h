#define CHILD 0

typedef struct instruction_struct {
	char *command;
	char **arguments;
} instruction;

instruction *create_instruction(char *, char **);
int destroy_instruction(instruction *);

int execute_commands(instruction **, int);
instruction **parse_command(char *);
