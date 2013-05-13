/* File: admin.h
 * Authors: Bas van den Heuvel & Bram van den Akker.
 *
 * This file contains constants and prototypes for admin.c. */

#define FIRST_INDEX 1
#define ADMIN_SIZE 1

long admin_make(long next_index, int used);
int admin_get_used(long admin);
long admin_get_next_index(long admin);
int in_block_space(long *mem, long block_index);
long get_block_size(long *mem, long block_index);
int free_block(long *mem, long block_index);
int get_block_count(long *mem);
int alloc_block(long *mem, long index, long size);
int merge_block(long *mem, long first_index, long second_index);
