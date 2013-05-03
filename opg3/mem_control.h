/* Authors:
 * -Bram van den Akker
 * -Bas van den Heuvel */

/* Find the size of a block in memory. Returns -1 if the index is not the start
 * of a block. */
long mem_block_size(long index);

/* Find the size of a gap in memory. Returns -1 if the index is not the start
 * of a gap. */
long mem_gap_size(long index);

/* Find the index of the first gap in memory. Returns -1 if no gap can be
 * found. */
long mem_find_first_gap(long index);
