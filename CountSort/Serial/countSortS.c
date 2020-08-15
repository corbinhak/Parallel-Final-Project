#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "mpi.h"

int main(int argc, char *argv[]){
    double startTime = MPI_Wtime();
    int rank, numranks;
    int n = 10000;
    int max = 100;
    int counter;

    //creates an array for the inputs
    int *list = (int*)malloc(n * sizeof(int));
    //creates an array of all the possible inputs
    int *match = (int*)malloc((max) * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Status stats;

    srand(time(NULL));

    printf("Input Array = [");
    for(int i = 0; i < n; i++){
        list[i] = rand() % (max+1);
        if(i != n-1){
            printf("%d,", list[i]);
        }
        else{
            printf("%d", list[i]);
        }
    }
    printf("]\n");

    for(int i = 0; i < max; i++){
        match[i] = 0;
    }

    //compares and calculates the final array
    int x;
    for(int i = 0; i < n; i++){
        x = list[i];
        match[x]++;
    }

    counter = 0;
    printf("Output Array = [");
    for(int i = 0; i <= max; i++){
        for(int j = match[i]; j > 0; j--){
            if(counter != n-1){
                printf(" %d,", i);
                counter++;
            }
            else{
                printf(" %d", i);
            }
        }
    }
    printf("]\n");

    double endTime = MPI_Wtime();
    double runtime = endTime - startTime;

    printf("Runtime: %f\n", runtime);

    MPI_Finalize();

    return 0;
}
