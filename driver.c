#include <stdlib.h>
#include <stdio.h>
#include "sudoku_generator.h"

/*
*   This is a small driver program to demonstrate the sudoku libary. 
*/


int main() {

    int size = 0;
    int blank_fields = 0; 

    printf("Enter the number of sudokus to generate: ");
    scanf("%d", &size);

    printf("Enter the number of blank fields the sudokus should have: ");
    scanf("%d", &blank_fields);

    // generate sudokus 
    short **sudokus = generate_sudokus(size, blank_fields);
    
    for (int i = 0; i < size; i++) {

        printf("-------------------------\n");
        printf("Sudoku number %d\n", i+1);
        printf("Blank fields: %d\n", sudoku_blank_fields(sudokus[i]));
        printf("\n");

        print_sudoku(sudokus[i]);
        
        // compute solution
        solve_sudoku(sudokus[i]);

        printf("\n\n");
        printf("Solution: \n");
        print_sudoku(sudokus[i]);

    }
    printf("-------------------------\n");

    // free memory
    free_sudoku_array(sudokus, size);

    return 0;
}

