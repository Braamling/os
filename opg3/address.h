#define ADDR_START 2
#define ADDR_MAX_INDEX 0
#define ADDR_COUNT_INDEX 1

long address_set(long block_index, int used);
int address_is_used(long address);
long get_index(long address);
int insert_address(long* mem, long addr_index, long address);
long get_free_address_slot(long *mem);
long get_address_max(long *mem);
long get_address_count(long *mem);
long get_address_end(long *mem);
int get_block_size(long *mem, long block_index);
int move_addresses_right(long *mem, long addr_index);
int move_addresses_left(long *mem, long addr_index);
int in_addr_space(long *mem, long addr_index);
int in_block_space(long *mem, long block_index);
int set_block_size(long *mem, long block_index, long size);
int remove_address(long *mem, long addr_index);
int alloc_mem(long *mem, long gap_addr, long request);
long address_set_used(long address, int used);
int set_block_size(long *mem, long block_index, long size);
