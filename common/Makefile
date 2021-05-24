# Makefile for 'common' module
#
# Nunchuck-Buccaneers
# CS50-Spring 2021
# 05/21/21
#

OBJS = grid.o pos2D.o mem.o visibility.o file.o map.o
LIB = common.a
LLIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../support
CC = gcc
MAKE = make

.PHONY: all clean test

all: $(LIB) gridtest visibilitytest

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

gridtest: $(OBJS) gridtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

visibilitytest: $(OBJS) visibilitytest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

grid.o: grid.h pos2D.h mem.h
visibility.o: visibility.h pos2D.h grid.h mem.h
pos2D.o: pos2D.h
mem.o: mem.h
map.o: map.h file.h pos2D.h grid.h mem.h gameInfo.h
file.o: file.h
gridtest.o: mem.o pos2D.o grid.o grid.h pos2D.h
visibilitytest.o: pos2D.o grid.o grid.h pos2D.h file.h mem.h

test-grid: gridtest
	valgrind --leak-check=full --show-leak-kinds=all ./gridtest

test-visibility: visibilitytest
	valgrind --leak-check=full --show-leak-kinds=all ./visibilitytest

clean:
	rm -rf *.dSYM
	rm -rf  *.o
	rm -rf gridtest
	rm -rf visibilitytest
