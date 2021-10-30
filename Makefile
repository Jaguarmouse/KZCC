CC=gcc
CFLAGS=-std=gnu11 -g -static -I ./include
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src%.c=dist%.o)

kzcc: $(OBJS)
	$(CC) -o kzcc $(OBJS) $(LDFLAGS)

$(OBJS): include/kzcc.h

test: kzcc
	./test.sh

clean:
	rm -f kzcc dist/*.o *~ tmp*

run:
	docker run --rm -it -v ${PWD}:/KZCC compilerbook

.PHONY: test clean run
