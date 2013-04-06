CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

bp: bp.c
	$(CC) $^ $(CFLAGS) -o $@

all: bp

clean:
	rm -f *.o

run: bp
	./bp