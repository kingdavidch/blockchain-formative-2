CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: blockchain

blockchain: main.o block.o blockchain.o sha256.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c blockchain.h block.h
	$(CC) $(CFLAGS) -c main.c

block.o: block.c block.h sha256.h
	$(CC) $(CFLAGS) -c block.c

blockchain.o: blockchain.c blockchain.h block.h
	$(CC) $(CFLAGS) -c blockchain.c

sha256.o: sha256.c sha256.h
	$(CC) $(CFLAGS) -c sha256.c

clean:
	rm -f *.o blockchain

.PHONY: all clean
