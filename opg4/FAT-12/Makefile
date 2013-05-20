CC=gcc
CFLAGS=-O2 -Wall -ansi -pedantic -lm -ggdb -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

FAT-12-reader: FAT-12-reader.c
	$(CC) $^ $(CFLAGS) -o $@

debug: debug.c FAT-12-reader.o
	$(CC) $^ $(CFLAGS) -o $@

all: next-fit

clean:
	rm -f FAT-12-reader.o

run: FAT-12-reader
	./FAT-12-reader disk-image3

dbg: debug
	./debug 