#!/bin/sh
gcc file.c hash.c -DSTANDALONE -DTEST -Wall
gcc file.c test.c -DSTANDALONE -DTEST -Wall -o test
