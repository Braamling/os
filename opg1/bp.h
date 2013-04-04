#define CHILD 0

typedef struct instruction_struct {
	char *command, *arguments;
} instruction;

instruction *create_instruction(char *, char *);
int destroy_instruction(instruction *);

int execute_command(unsigned char *, unsigned char *);
int parse_command(unsigned char *);
