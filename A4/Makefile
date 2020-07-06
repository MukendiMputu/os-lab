CC ?= gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGETS = 4a.c 4b.c 4extended.c nextfit.c test_4a.c test_4b.c test_4extended.c
OBJECTS = ${TARGETS:.c=.o}

all: ${OBJECTS}

# Die Datei deps.mk gibt an, welche Objektdateien von welchen C-Dateien und
# Headern abhängen. Sie wurde mit "gcc -MM *.c > deps.mk" generiert.
include deps.mk

test_4a: test_4a.o 4a.o nextfit.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

test_4b: test_4b.o 4b.o 4a.o nextfit.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

test_4extended: test_4extended.o 4extended.o nextfit.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

4a: test_4a
	@./test_4a

4b: test_4b
	@./test_4b

4extended: test_4extended
	@./test_4extended

clean:
	rm -f test_4a test_4b test_4extended ${OBJECTS}

.PHONY: 4a 4b 4extended all clean
