CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

scanner: scanner.c
	$(CC) $(CFLAGS) -o $@

all: scanner

clean:
	rm -f *.o
