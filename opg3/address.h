#define ADDR_START 2
#define ADDR_MAX_INDEX 0
#define ADDR_COUNT_INDEX 1

long address_set(long index, int used);
int address_is_used(long address);
long get_index(long address);
int insert_address(long* mem, long index, long address) {
long get_free_address_slot(long *mem);
long get_address_max(long *mem);
long get_address_count(long *mem);
long get_address_end(long *mem);
int get_block_size(long *mem, long index);