CC=gcc
CFLAGS = -lm

mips: mips.c
	$(CC) mips.c $(CFLAGS) -o mips
