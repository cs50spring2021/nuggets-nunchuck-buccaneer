# Makefile for 'common' module
#
# Nunchuck-Buccaneers
# CS50-Spring 2021
# 05/21/21
#

OBJS = grid.o pos2D.o mem.o map.o
TESTS = gridtest.o
LIB = common.a
LLIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

.PHONY: all clean test

all: $(LIB) gridtest

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

gridtest: $(OBJS) $(TESTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

grid.o: grid.h pos2D.h mem.h
pos2D.o: pos2D.h
mem.o: mem.h
map.o: map.h file.h pos2D.h grid.h mem.h
gridtest.o: mem.o pos2D.o grid.o grid.h pos2D.h pos2D.h

test-grid:
	valgrind --leak-check=full --show-leak-kinds=all ./gridtest

clean:
	rm -rf *.dSYM
	rm -rf  *.o
	rm -rf gridtest
