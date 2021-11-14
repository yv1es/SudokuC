#ifndef SUDOKU_GEN
#define SUDOKU_GEN

/*
*   This module provides some functions for generating and solving sudoku puzzles. 
*   A sudoku is represented as an array of 81 short values. 
*   Empty fields in the puzzle are represented as 0's. 
*
*   Remember to call the free_sudoku_array function when the sudokus are no longer used to avoid memory leaks.  
*/


/*
*   This function returns an array of sudoku puzzles. 
*   number_of_blanks is equal to the number of blank fields in the generated sudokus. 
*
*   Note that the generated sudokus have unique solutions (like proper sudokus should have).
*   The minimum number of values a sudoku can have and still be uniquely solvable is 17. 
*   Hence number_of_blanks must be greater equal 64.
*
*   Also note that when number_of_blanks increases the computational cost increases exponentially.
*   This function will run the computation until a sudoku that matches the blank field constraint is found.
*   Depending on the hardware the computation time for big number_of_blanks might become impractical. 
*/
short **generate_sudokus(int size, int number_of_blanks);


/*
*   This function solves the given soduku in place. 
*/
int solve_sudoku(short*);


/*
*   This function frees the memory occupied by an array of sudokus. 
*   It should be called when the sudokus are no longer needed to avoid memory leaks. 
*/
void free_sudoku_array(short**, int size);


/*
*   This function returns the number of blank fields   
*/
int sudoku_blank_fields(short*);

/*
*   This function prints a sudoku to the command line. 
*/
void print_sudoku(short*);


#endif /*SUDOKU_GEN */