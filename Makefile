# Makefile for 'crawler' module
#
# John "James" Utley
# CS50, 4/26/2021
# Made to create and test the crawler module
C = ./common
S = ./support
OBJS = server.o
LIBS = 
LLIBS = $C/common.a $S/support.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$S -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
all: server

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

server.o: server.c $C/pos2D.h $S/message.h

.PHONY: test valgrind clean

#  Tests
#test: crawler
#	bash -v testing.sh
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server
	rm -f client

