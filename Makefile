# Makefile for 'nuggets' module
#
# nunchuck-buccaneers
# CS50, 5/29/2021
# Made to create and test the nuggets module
C = ./common
S = ./support
OBJS = server.o network.o serverCmds.o clientCmds.o
OBJS2 = client.o network.o clientCmds.o serverCmds.o
OBJS3 = network.o networktest.o clientCmds.o serverCmds.o
LIBS = -lm -lncurses
LLIBS = $C/common.a $S/support.a
#Uncomment for server tests
#DEFINES = -DTESTING
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(DEFINES) -I$S -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all -v
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
clienttest: clienttest.o network.o serverCmds.o clientCmds.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
networktest: $(OBJS3) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

server.o: server.c $C/pos2D.h $S/message.h $C/gameInfo.h network.h $C/map.h
client.o: client.c $C/pos2D.h network.h
network.o: network.h $C/mem.h $C/pos2D.h $C/file.h $S/message.h clientCmds.h serverCmds.h
clientCmds.o: clientCmds.h $C/pos2D.h $C/mem.h $C/pos2D.h
serverCmds.o: serverCmds.h server.c $C/pos2D.h $S/message.h $C/gameInfo.h $C/map.h
servertest.o: servertest.c 
clienttest.o: clienttest.c $C/file.h $C/grid.h clientCmds.h network.h
networktest.o: networktest.c network.h $C/mem.h $C/pos2D.h $C/file.h $S/message.h clientCmds.h serverCmds.h.
PHONY: test valgrind clean

#  Tests
testModuleAndMains:
	make clean
	make
	make test-grid -C common
	make test-visibility -C common
	make test-map -C common
	make test-gameInfotest -C common
	make test-pos2Dtest -C common
testCmdsAndNetwork:
	make clean
	make -i
	make servertest
	make networktest
	./servertest
	./networktesttest
Client:
	make clean
	make -i
	make clienttest
	./clienttest
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server
	rm -f client
	rm -rf client
	make -C common clean
	make -C support clean
	rm -rf networktest
	rm -rf servertest
	rm -rf vgcore.*
	rm -rf core
