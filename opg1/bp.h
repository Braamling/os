#define CHILD 0
#define MAX_INSTRUCTIONS 10

typedef struct instruction_struct {
	char *command;
	char **arguments;
	struct instruction_struct *child;
} instruction;

instruction *create_instruction(char *, char **);
int destroy_instruction(instruction *);

int execute_commands(instruction *, int);

instruction *parse_command(char *);
char *read_line();
