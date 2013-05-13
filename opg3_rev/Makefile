CC=gcc
CFLAGS=-O2 -Wall -ansi -pedantic -lm -ggdb -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

next-fit: next_fit.o mem_alloc.o used_bit.o mem_test2-64.o
	$(CC) $^ $(CFLAGS) -o $@

best-fit: best_fit.o mem_alloc.o used_bit.o mem_test2-64.o
	$(CC) $^ $(CFLAGS) -o $@

run: next-fit
	./next-fit
