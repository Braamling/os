long mem_block_size(long* mem, long index) {
	long end;

	end = mem_seek_block_end(long index);

	if(end = -1){
		return -1;
	}

	return end - index;
}

long mem_seek_block_end(long* mem, long index) {
	long i;

	/* Go through the memory until and end of a block is found. */
	for (i = index; i <= MEM_SIZE; i++) {
		if (mem[i] == MEM_BLOCK_START) {
			return -1;
		}
		if (mem[i] == MEM_BLOCK_END) {
			return i;
		}
	}

	return -1;
}