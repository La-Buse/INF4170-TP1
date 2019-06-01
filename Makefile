CC=gcc
CLFAGS=

mips_to_binary: mips_to_binary.c
	$(CC) -o mips_to_binary mips_to_binary.c

