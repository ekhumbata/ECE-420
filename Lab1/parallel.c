#include "lab1_IO.h"
#include "timer.h"
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int **A;
int **B;
int **C;
int size;
int numThreads;
const int DEBUG = 0;

void *calculate_submatrix(void *rank) {

    int k = (int) rank; // k is equivalent to the thread ID

    // these bounds determine how to partition the matrix into blocks
    int x = floor(k / (int) sqrt(numThreads));
    int y = k % (int) sqrt(numThreads);

    if (DEBUG == 1) {
        printf("RANK %d\n", k);
        printf("x is: %d\n", x);
        printf("y is: %d\n", y);
    }

    // calculate submatrix block width for this thread
    int block_width = size / sqrt(numThreads);

    // calculate this thread's index bounds
    int i_start = block_width * x;
    int i_end = block_width * (x+1) - 1;

    int j_start = block_width * y;
    int j_end = block_width * (y+1) - 1;

    // Calculate submatrix
    for(int i = i_start; i < i_end; i++) {
        for(int j = j_start; j < j_end; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k ++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    double time_start, time_end;

    // check number of threads is positive integer
    if (atoi(argv[1]) < 1) {
        printf("Number of threads entered was invalid. Using a single thread.\n");
        numThreads = 1;
    }
    else {
        numThreads = atoi(argv[1]);
        printf("Using %d threads...\n", numThreads);
    }

    // allocate space for our threads
    pthread_t *thread_handles = malloc(numThreads * sizeof(pthread_t));

    Lab1_loadinput(&A, &B, &size);

    printf("size is: %d\n", size);

    // initialize C
    C = malloc(size * sizeof(int*));

    for (int i = 0; i < size; i++) {
        C[i] = malloc(size * sizeof(int*));
    }

    GET_TIME(time_start);

    for (int thread = 0; thread < numThreads; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, calculate_submatrix, (void *)thread);
    }

    // wait for all threads to finish
    for (int thread = 0; thread < numThreads; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    GET_TIME(time_end);

    Lab1_saveoutput(C, &size, time_end - time_start);

    // free allocated matrices
    // gotta call free as many times as we call malloc
    for (int i = 0; i < size; i++)
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);
    free(thread_handles);

    return 0;
}