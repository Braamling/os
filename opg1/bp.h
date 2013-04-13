#define CHILD 0
#define MAX_INSTRUCTIONS 10
#define TERMINATE 1
#define ALLOW_CD 1
#define DISALLOW_CD 0

typedef struct instruction_struct {
	char **command;
	struct instruction_struct *child;
} instruction;

instruction *create_instruction(char **);
int destroy_instruction(instruction *);
int execute_commands(instruction *, int);
instruction *parse_command(char *);
char *read_line();
int run_line(char *, int);
int execute_file(char *);
char *trim_start(char *);
char *build_prompt();
void prompt_sigint_handler(int);
void piping_sigint_handler(int);
