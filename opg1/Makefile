CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -ggdb -lreadline -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

bp: bp.c
	$(CC) $^ $(CFLAGS) -o $@

read_file: read_file.c
	$(CC) $^ $(CFLAGS) -o $@

all: bp

clean:
	rm -f *.o

run: bp
	./bp
