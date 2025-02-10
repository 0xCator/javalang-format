
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -g

all: 
	$(CC) $(CFLAGS) -o javalang-fromat ./src/*.c
