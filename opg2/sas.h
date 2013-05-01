pcb *mem_wait_proc;

typedef enum SCHEDULER {ROUND_ROBIN, MULTI_LEVEL} scheduler_type;

static void multi_level_scheduler();

static void round_robin();
