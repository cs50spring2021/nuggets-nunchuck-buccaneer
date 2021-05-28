# Makefile for 'crawler' module
#
# John "James" Utley
# CS50, 4/26/2021
# Made to create and test the crawler module
C = ./common
S = ./support
OBJS = server.o servertest.o
OBJS2 = client.o clienttest.o
LIBS = -lm -lncurses
LLIBS = $C/common.a $S/support.a

#Uncomment for server tests
DEFINES = -DTESTING
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(DEFINES) -I$S -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all:
	make -C support
	make -C common
	make client

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

client: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

servertest: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clienttest: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


server.o: server.c $C/pos2D.h $S/message.h $C/gameInfo.h $S/network.h
client.o: client.h $C/pos2D.h 

servertest.o: servertest.c

clienttest.o: clienttest.c $C/file.h $C/grid.h

.PHONY: test valgrind clean

#  Tests
testserver: server servertest
	./servertest
#bash -v testing.sh

testclient: client clienttest
	./clienttest
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server
	rm -f client
	rm -rf client
	make -C common clean
	make -C support clean
