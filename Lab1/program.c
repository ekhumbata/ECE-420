/**
 * OUTDATED
*/


#include "lab1_IO.c"
#include "timer.h"
#include <math.h>
#include <pthread.h>

// minimum number of threads is 1
int numThreads = 1;
int size;
int DEBUG = 0;


void *matrixMultiplication(void* rank) {
    int k = (int) rank; // k is equivalent to the thread ID

    // these bounds determine how to partition the matrix into blocks
    int x = floor(k / (int) sqrt(numThreads));
    int y = k % (int) sqrt(numThreads);

    if (DEBUG == 1) {
        printf("RANK %d\n", k);
        printf("x is: %d\n", x);
        printf("y is: %d\n", y);
    }

    int i_start = size / sqrt(numThreads) * x;
    int i_end = size / sqrt(numThreads) * (x+1) - 1;

    int j_start = size / sqrt(numThreads) * y;
    int j_end = size / sqrt(numThreads) * (y+1) - 1;


    for(int i = i_start; i < i_end; i++) {

        for(int j = j_start; j < j_end; j++) {
            // matrix multiplication happens here
        }
    }



    return NULL;

}


int main(int argc, char* argv[]) {
    int **A;  // pointer to the matrix A
    int **B;  // pointer to the matrix B
    int **C;     //pointer to the result matrix 
    //int n;    // matrix size
    int i, j, k, thread;   //loop counters
    double start, end;  //program timers
    pthread_t* thread_handles; // array holding all thread handles

    // check number of threads is positive integer
    if(atoi(argv[1]) < 1){
        printf("Number of threads entered was invalid. Using a single thread.");
    } else {
        numThreads = atoi(argv[1]);
        printf("Using %d threads...\n",numThreads);
    }

    // allocate space for our threads
    thread_handles = malloc(numThreads * sizeof(pthread_t));

    Lab1_loadinput(&A, &B, &n);

    printf("n is: %d\n", n);

    // order of indices is [row][column]
    //printf("printing first element of A: %d\n", ((A)[2][1])); // print element of loaded matrix

    //allocate row memory for C
    C = malloc(n * sizeof(int*)); 

    //allocate column memory for C
    for (i = 0; i < n; i++) {
      (C)[i] = malloc(n * sizeof(int));
    }
    
    //calculation starting, mark the time
    GET_TIME(start);

    // create our threads, passing in the thread rank(a.k.a. the thread ID number)
    for(thread = 0; thread < numThreads; thread++) {
        pthread_create(&thread_handles[thread], NULL, matrixMultiplication, (void*) thread);
    }

    // load up C
    //iterate through row
    for (i = 0; i < n; i++) {
        //iterate through column
        for (j = 0; j < n; j++) {
            //init to zero
            C[i][j] = 0;
            
            //load up C
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
                //printf("%d\n", A[i][k]);
            }
 
            //printf("%d\t", C[i][j]);
        }
 
        //printf("\n");
    }

    // wait for all threads to finish
    for(thread = 0; thread < numThreads; thread++) { // TODO - create thread_handles array and initialize numThreads of thread objects
        pthread_join(thread_handles[thread], NULL);
    }

    //calculation complete, mark the time
    GET_TIME(end);


        
    Lab1_saveoutput(C, &n, end-start);

    // free allocated matrices and thread handles
    free(*A);
    free(*B);
    free(*C);
    free(thread_handles);

    return 0;
}