CC=gcc
CFLAGS=-std=gnu11 -g -static -I ./include
SRC_DIR=src
OBJ_DIR=build
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

kzcc: $(OBJS)
	$(CC) -o kzcc $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: kzcc
	./test.sh

clean:
	rm -f kzcc $(OBJS) *~ tmp*

run:
	docker run --rm -it -v ${PWD}:/KZCC compilerbook

.PHONY: test clean run
