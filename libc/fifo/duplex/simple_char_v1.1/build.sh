#!/bin/sh

gcc fifo_server.c -lpthread -o server_fifo
gcc fifo_client.c -lpthread -o client_fifo
