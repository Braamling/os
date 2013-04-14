CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb -lreadline -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

bp: bp.c bp_instruction.o bp_handler.o
	$(CC) $^ $(CFLAGS) -o $@

all: bp

clean:
	rm -f *.o

run: bp
	./bp
