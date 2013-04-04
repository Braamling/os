#define CHILD 0

typedef struct instruction_struct {
	char *command, **arguments;
} instruction;

instruction *create_instruction(char *, char **);
int destroy_instruction(instruction *);

int execute_command(char *, char *);
instruction **parse_command(char *);
