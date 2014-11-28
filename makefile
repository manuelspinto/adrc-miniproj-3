CC=gcc
CFLAGS=-g -c -Wall -pedantic
LIBS=-lm

default: connectivity

connectivity: connectivity.o
	$(CC) -o connectivity connectivity.o $(LIBS)

connectivity.o: connectivity.c connectivity.h
	$(CC) $(CFLAGS) connectivity.c

clean::
	rm -f *.o core a.out connectivity *~
