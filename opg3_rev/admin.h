#define FIRST_INDEX 1

long admin_make(long next_index, int used);
int admin_get_used(long admin);
long admin_get_next_index(long admin);
int in_block_space(long *mem, long block_index);
long get_block_size(long *mem, long block_index);
long free_block(long *mem, long block_index);
int get_block_count(long *mem);
