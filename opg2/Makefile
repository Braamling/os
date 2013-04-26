CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb -lreadline -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

sas: sas.c mt19937ar.o simul.o mem_alloc.o pcb_control.o multilevel.o
	$(CC) $^ $(CFLAGS) -o $@

all: sas

clean:
	rm -f *.o

run: sas
	./sas
