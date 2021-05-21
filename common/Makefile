# Makefile for 'common' module
#
# Will French
# Nunchuck-Buccaneers
# CS50-Spring 2021
# 05/21/21
#

OBJS = grid.o
LIB = common.a
LLIBS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) 
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

.PHONY: all clean

grid.o: grid.h pos2D.h

clean:
	rm -rf *.dSYM
	rm -rf  *.o
