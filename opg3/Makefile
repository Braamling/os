CC=gcc
CFLAGS=-O2 -Wall -ansi -pedantic -lm -ggdb -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

next-fit: next_fit.o mem_alloc.o admin.o mem_test2-64.o
	$(CC) $^ $(CFLAGS) -o $@

best-fit: best_fit.o mem_alloc.o admin.o mem_test2-64.o
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -f admin.o
	rm -f mem_alloc.o
	rm -f next_fit.o
	rm -f best_fit.o
	rm -f next-fit
	rm -f best-fit

next: clean next-fit
	./next-fit

best: clean best-fit
	./best-fit
