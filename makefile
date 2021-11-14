CC=gcc
CFLAGS= -O3 -Wall
DEPS = sudoku_generator.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

driver: driver.c sudoku_generator.c 
	$(CC) -o driver driver.c sudoku_generator.c 