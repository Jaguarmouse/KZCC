CFLAGS=-std=c11 -g -static

kzcc: kzcc.c

test: kzcc
	./test.sh

clean:
	rm -f kzcc *.o *~ tmp*

.PHONY: test clean
