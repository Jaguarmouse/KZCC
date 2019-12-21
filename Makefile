CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

kzcc: $(OBJS)
	$(CC) -o kzcc $(OBJS) $(LDFLAGS)

$(OBJS): kzcc.h

test: kzcc
	./test.sh

clean:
	rm -f kzcc *.o *~ tmp*

.PHONY: test clean
