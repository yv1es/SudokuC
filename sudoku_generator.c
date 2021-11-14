#include "sudoku_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// declaraion of local functions 
static int possible_entry(short*, int, int);
static int generate_random_number();
static int randomly_solve_sudoku(short*);

/*
*   This function allocates memory for a sudoku array of 'size' sudokus 
*/
short **allocate_sudoku_array(int size) {
    // allocate sudoku array
    short **sudoku_array = malloc(sizeof(short*) * size);
    if (sudoku_array == NULL) { 
        printf("[ERROR] Heap allocation for sudoku arrary failed");
        return NULL; 
    }

    // allocate sudokus 
    for (int i = 0; i < size; i++) {
        short *sudoku = calloc(81, sizeof(short)); 
        if (sudoku == NULL) { 
            printf("[ERROR] Heap allocation for sudoku failed");
            return NULL; 
        }

        sudoku_array[i] = sudoku;
    }
    return sudoku_array;
}


/*
*   Returns a random integer smaller than limit 
*/
int generate_random_number(int limit) {
    return rand() % limit; 
}


/*
*   This fuction returns true if 'entry' at 'index' in the sudoku does not conflict with any other entry
*/
static int possible_entry(short *sudoku, int index, int entry) {

    int row = index / 9; 
    int col = index % 9; 

    // check row 
    for (int i = 0; i < 9; i++) {
        if (sudoku[row*9 + i] == entry) { return 0; }
    }

    // check column
    for (int i = 0; i < 9; i++) {
        if (sudoku[col + i*9] == entry) { return 0; }
    }

    // check box 
    int box_row = (row / 3) * 3; 
    int box_col = (col / 3) * 3; 
    int box_index = box_row*9 + box_col; 

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[j + i*9 + box_index] == entry) { return 0; }
        }
    }
    return 1; 
}



/*
*  This function counts the possible solutions that exist for a given sudoku 
*/
int count_sudoku_solutions(short *sudoku) {
    
    int solutions = 0; 

    // loop through empty fields 
    for (int i = 0; i < 81; i++) {
        if (sudoku[i] != 0) { continue; }

        // empty field found at index i
        for (int p = 1; p <= 9; p++) {
            if (possible_entry(sudoku, i, p)) {

                // found a possible entry, fill the entry and explore the possible solutions 
                sudoku[i] = p; 
                solutions += count_sudoku_solutions(sudoku);
                sudoku[i] = 0; 
            }
        }

        // all possibilities explored 
        return solutions; 
    }

    // no empty spots found hence this is a solution 
    return 1; 
}



/*
*   This funcion fills a sudoku in a random manner whereas 
*   the solve_sudoku function will choose the smallest possible entry 
*   for an empty field among all possible entries. 
*/
static int randomly_solve_sudoku(short *sudoku) {

    for (int i = 0; i < 81; i++) {
        
        if (sudoku[i] != 0) { continue; }

        // generate random offset
        int r = generate_random_number(9);

        // empty field found at index i
        for (int p = 1; p <= 9; p++) {

            int entry = (p+r)%9 + 1; 

            if (possible_entry(sudoku, i, entry)) {

                // found a possible entry 
                sudoku[i] = entry; 
                if (randomly_solve_sudoku(sudoku)) {
                    return 1; 
                }
                
                // the entry was not correct, hence reset to 0
                sudoku[i] = 0;
            }
        }

        // no possible entry found
        return 0; 

    }
    // no empty spots
    return 1; 
}



/*
*   This function returns an array of sudoku puzzles. 
*   number_of_blanks is equal to the number of fields on the generated sudokus. 
*/
short **generate_sudokus(int size, int number_of_blanks) {

    if (number_of_blanks > 64) {
        printf("Error: no sudoku with more than 64 blank fields exist");
    }

    short **sudoku_array = allocate_sudoku_array(size);

    // loop though sudoku array
    for (int i = 0; i < size; i++) {
        
        // fill the soduku with a random solution
        randomly_solve_sudoku(sudoku_array[i]);

        int blanks = 0; 

        while (blanks < number_of_blanks) {
            
            // bool to keep track if a entry to remove was found 
            int success = 0; 

            // choose a random field to be erased
            int r = generate_random_number(80); 
            for (int j = 0; j < 81; j++) {
                int index = (r+j)%81;

                if (sudoku_array[i][index] == 0) { continue; }

                // erase field 
                int old_entry = sudoku_array[i][index];
                sudoku_array[i][index] = 0;  

                // test if the solution is no longer unique
                if (count_sudoku_solutions(sudoku_array[i]) > 1) {
                    
                    // restore and try the next index
                    sudoku_array[i][index] = old_entry; 
                    continue; 
                }
                else {
                    // removed the entry while keeping unique solvability
                    success = 1; 
                    blanks++; 
                    break; 
                }
            }

            if (!success) {
                // for this configuration no entry can be removed hence start again with an other random sudoku
                blanks = 0; 
                for (int k = 0; k < 81; k++) { sudoku_array[i][k] = 0; }
                randomly_solve_sudoku(sudoku_array[i]); 
            }
        }
    }
    return sudoku_array; 
}



/*
*   This function solves the given soduku in place. 
*/
int solve_sudoku(short *sudoku) {

    for (int i = 0; i < 81; i++) {
        if (sudoku[i] != 0) { continue; }

        // empty field found at index i
        for (int p = 1; p <= 9; p++) {
            if (possible_entry(sudoku, i, p)) {

                // found a possible entry 
                sudoku[i] = p; 
                if (solve_sudoku(sudoku)) {
                    return 1; 
                }
                
                // the entry was not correct, hence reset to 0
                sudoku[i] = 0;
            }
        }

        // no possible entry found
        return 0; 

    }
    // no empty spots
    return 1; 
}


/*
*   This function frees the memory occupied by an array of sudokus. 
*   It should be called when the sudokus are no longer needed to avoid memory leaks. 
*/
void free_sudoku_array(short** sudokus, int size)  {

    // free sudokus 
    for (int i = 0; i < size; i++) {
        free(sudokus[i]); 
    }

    // free sudoku array
    free(sudokus);
}


/*
*   This function returns the number of blank fields   
*/
int sudoku_blank_fields(short *sudoku) {
    int blanks = 0; 
    for (int i = 0; i < 81; i++) {
        if (sudoku[i] == 0) { blanks++; }
    }
    return blanks; 
}


/*
*   This function print a sudoku to the command line. 
*/
void print_sudoku(short *sudoku) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            
            int entry = sudoku[i*9 + j];
            if (entry == 0) {
                printf("  ");
            }
            else {
                printf(" %hi", entry);
            }
            
            if (j < 8 && j % 3 == 2) {
                printf(" |");
            }
        }
        printf("\n");
        if (i < 8 && i % 3 == 2) {
            printf("-----------------------\n");
        }
    }
}

