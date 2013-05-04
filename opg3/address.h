#define ADDR_START 2
#define ADDR_MAX_INDEX 0
#define ADDR_COUNT_INDEX 1

long address_set(long index, int used);

int address_is_used(long address);

long get_index(long address);

int get_block_size(long *mem, long index);