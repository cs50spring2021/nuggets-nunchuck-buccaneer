# Makefile for 'common' module
#
# Nunchuck-Buccaneers
# CS50-Spring 2021
# 05/21/21
#

S = ../support
OBJS = grid.o pos2D.o mem.o visibility.o file.o gameInfo.o map.o

LIB = common.a
LLIBS = $S/support.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$S
CC = gcc
MAKE = make

.PHONY: all clean test

all: $(LIB) gridtest visibilitytest maptest gameInfotest

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

gridtest: $(OBJS) $(LLIBS) gridtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

visibilitytest: $(OBJS) $(LLIBS) visibilitytest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

maptest: $(OBJS) $(LLIBS) maptest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

gameInfotest: $(OBJS) $(LLIBS) gameInfoTest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

pos2Dtest: $(OBJS) $(LLIBS) pos2Dtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -lm -o $@

grid.o: grid.h pos2D.h mem.h
visibility.o: visibility.h pos2D.h grid.h mem.h
pos2D.o: pos2D.h
mem.o: mem.h
map.o: map.h file.h pos2D.h grid.h mem.h gameInfo.h playerInfo.h
file.o: file.h
gridtest.o: mem.o pos2D.o grid.o grid.h pos2D.h
visibilitytest.o: pos2D.o grid.o grid.h pos2D.h file.h mem.h
maptest.o: mem.o map.h grid.h
gameInfo.o: gameInfo.h mem.h pos2D.h grid.h $S/message.h visibility.h map.h
playerInfo.o: $S/message.h

test-grid: gridtest
	valgrind --leak-check=full --show-leak-kinds=all ./gridtest

test-visibility: visibilitytest
	valgrind --leak-check=full --show-leak-kinds=all ./visibilitytest

test-map: maptest
	valgrind --leak-check=full --show-leak-kinds=all ./maptest

test-gameInfotest: gameInfotest
	valgrind --leak-check=full --show-leak-kinds=all ./gameInfotest

test-pos2Dtest: pos2Dtest
	valgrind --leak-check=full --show-leak-kinds=all ./pos2Dtest

clean:
	rm -rf *.dSYM
	rm -rf  *.o
	rm -rf gridtest
	rm -rf visibilitytest
	rm -rf maptest
	rm -rf pos2Dtest
	rm -rf common.a
	rm -rf gameInfotest