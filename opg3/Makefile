CC=gcc
CFLAGS=-O2 -Wall -ansi -pedantic -lm -ggdb -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

next-fit: next-fit.c mem_test2-64.o
	$(CC) $^ $(CFLAGS) -o $@

all: next-fit

clean:
	rm -f next-fit.o

run: next-fit
	./next-fit
