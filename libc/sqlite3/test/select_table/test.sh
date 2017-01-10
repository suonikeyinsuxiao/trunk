#!/bin/sh

gcc table.c -I /home/frank/fujun/sqlite3/usr/local/include -L /home/frank/fujun/sqlite3/usr/local/lib -lsqlite3 -o table
