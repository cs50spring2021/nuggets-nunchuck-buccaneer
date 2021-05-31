#!/bin/bash
#
# testing.sh - Tests the server for Nuggets and prints results to stdout
#
# John "James" Utley, 5/23/2021
#
# TESTING - Just doing Edge Cases
#
# Too few args
./server 1
#
# Too Many Args
./server 1 2 3
#
# Bad Seed Test
./server maps/hole.txt 23ad45sdf235
#
# Double Seed Test
./server maps/hole.txt 214.3645
#
# Negative Seed Test
./server maps/hole.txt -214
#
# Non-existant Map Test
./server maps/what.txt 23235
#
# Unreadable Map Test
./server maps/unreadable.txt 23235
#
# Client Too Few Args
./client
#
# Client Too Many Args
./client host 324 name what
#
# Client Invalid Port
./client host 3a24 name 