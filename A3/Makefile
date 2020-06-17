CC = gcc
CFLAGS = -std=c11 -Wall -Wpedantic -D_POSIX_C_SOURCE=200112L

all: a b c

a: vorgabe.o a3_a.o
	$(CC) -pthread -o a3_a $^

b: vorgabe.o a3_b.o
	$(CC) -pthread -o a3_b $^

c: vorgabe.o a3_c.o
	$(CC) -pthread -o a3_c $^

.c.o:
	$(CC) -pthread $(CFLAGS) -c -o $@ $^

clean:
	rm -f a3_a.o a3_b.o a3_c.o vorgabe.o a3_a a3_b a3_c

.PHONY: all clean
