kzcc: kzcc.c

test: kzcc
	./test.sh

clean:
	rm -f kzcc *.o *~ tmp*
