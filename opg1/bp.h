#define CHILD 0
#define MAX_INSTRUCTIONS 10
#define TERMINATE 1

typedef struct instruction_struct {
	char **command;
	struct instruction_struct *child;
} instruction;

instruction *create_instruction(char **);
int destroy_instruction(instruction *);

int execute_commands(instruction *, int);

instruction *parse_command(char *);
char *read_line();

void make_user_friendly(char *cwd);
