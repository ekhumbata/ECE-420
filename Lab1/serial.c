#include "lab1_IO.h"
#include "timer.h"
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int **A;
    int **B;
    int **C;  //pointer to the result matrix 
    int size;
    double start, end;  //program timers

    Lab1_loadinput(&A, &B, &size);
    printf("size is: %d\n", size);

    //allocate row memory for C
    C = malloc(size * sizeof(int*)); 

    //allocate column memory for C
    for (int i = 0; i < size; i++) {
        C[i] = malloc(size * sizeof(int));
    }
    
    GET_TIME(start);

    // load up C
    //iterate through row
    for (int i = 0; i < size; i++) {
        //iterate through column
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            
            // calculate matrix element i, j
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    GET_TIME(end);
        
    Lab1_saveoutput(C, &size, end-start);

    // free allocated matrices
    // gotta call free as many times as we call malloc
    for (int i = 0; i < size; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}