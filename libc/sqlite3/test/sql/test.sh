#!/bin/sh

gcc sql.c test.c -I /home/frank/test/sqlite3/usr/local/include -L /home/frank/test/sqlite3/usr/local/lib -lsqlite3 -o sql
gcc sql.c test_pre.c -I /home/frank/test/sqlite3/usr/local/include -L /home/frank/test/sqlite3/usr/local/lib -lsqlite3 -o sql_pre
