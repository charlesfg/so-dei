CC = gcc
CFLAGS = -Wall -O0 -g
LDFLAGS = -lcrypto

all: pow_test


hash: hash_block_sample.c pow.o  deichain.h
	$(CC) $(CFLAGS) -o hash_sample hash_block_sample.c pow.o $(LDFLAGS)

pow_test: main.o pow.o  deichain.h
	$(CC) $(CFLAGS) -o pow_test main.o pow.o $(LDFLAGS)

pow.o: pow.c pow.h deichain.h
	$(CC) $(CFLAGS) -c pow.c

main.o: main.c pow.h deichain.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o pow_test