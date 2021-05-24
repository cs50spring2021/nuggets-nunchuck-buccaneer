# Makefile for 'common' module
#
# Nunchuck-Buccaneers
# CS50-Spring 2021
# 05/21/21
#
S = ../support
OBJS = grid.o pos2D.o mem.o visibility.o file.o gameInfo.o
LIB = common.a
LLIBS = $S/support.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$S
CC = gcc
MAKE = make

.PHONY: all clean test

all: $(LIB) gridtest visibilitytest gameInfo

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

gridtest: $(OBJS) gridtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

visibilitytest: $(OBJS) visibilitytest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@
	
gameInfo: $(OBJS) $(LLIBS) gameInfo.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

grid.o: grid.h pos2D.h mem.h
visibility.o: visibility.h pos2D.h grid.h mem.h
pos2D.o: pos2D.h
mem.o: mem.h
file.o: file.h
gridtest.o: mem.o pos2D.o grid.o grid.h pos2D.h pos2D.h
visibilitytest.o: pos2D.o grid.o grid.h pos2D.h file.h mem.h
gameInfo.o: mem.h pos2D.h grid.h $S/message.h visibility.h map.h


test-grid: gridtest
	valgrind --leak-check=full --show-leak-kinds=all ./gridtest

test-visibility: visibilitytest
	valgrind --leak-check=full --show-leak-kinds=all ./visibilitytest

clean:
	rm -rf *.dSYM
	rm -rf  *.o
	rm -rf gridtest
	rm -rf visibilitytest
	rm -rf common.a
