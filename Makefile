# Makefile for 'crawler' module
#
# John "James" Utley
# CS50, 4/26/2021
# Made to create and test the crawler module
C = ./common
S = ./support
OBJS = server.o network.o serverCmds.o clientCmds.o
OBJS2 = client.o network.o clientCmds.o serverCmds.o
LIBS = -lm -lncurses
LLIBS = $C/common.a $S/support.a

#Uncomment for server tests
#DEFINES = -DTESTING
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(DEFINES) -I$S -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all:
	make -C support
	make -C common
	make server
	make client

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

client: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

servertest: servertest.o network.o serverCmds.o clientCmds.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clienttest: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


server.o: server.c $C/pos2D.h $S/message.h $C/gameInfo.h network.h $C/map.h
client.o: client.c $C/pos2D.h network.h
network.o: network.h $C/mem.h $C/pos2D.h $C/file.h $S/message.h clientCmds.h serverCmds.h
clientCmds.o: clientCmds.h $C/pos2D.h $C/mem.h $C/pos2D.h
serverCmds.o: serverCmds.h server.c $C/pos2D.h $S/message.h $C/gameInfo.h $C/map.h

servertest.o: servertest.c

clienttest.o: clienttest.c $C/file.h $C/grid.h clientCmds.h

.PHONY: test valgrind clean

#  Tests
testserver: servertest
	./servertest
#bash -v testing.sh

Vtestserver: servertest
	$(VALGRIND) ./servertest

testclient: clienttest
	$(VALGRIND) ./clienttest

runserver: server
	$(VALGRIND) ./server maps/hole.txt 4
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server
	rm -f client
	rm -rf client
	make -C common clean
	make -C support clean
