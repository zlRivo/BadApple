CC=gcc
LD=-lncurses

default:
	$(CC) -g -o main main.c $(LD)
