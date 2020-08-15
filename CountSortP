#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "mpi.h"

int main(int argc, char *argv[]){
    double startTime = MPI_Wtime();
    int n = 10000;
    int rank, numranks;
    int num, counter;
    int max = 100;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Status stats;

    //creates an array for the inputs
    int *list = (int*)malloc(n * sizeof(int));
    //creates an array of all the possible inputs
    int *match = (int*)malloc((max) * sizeof(int));
    //creates an array for the output
    int *result = (int*)malloc((max) * sizeof(int)); 
    //creates arrays for MPI usage
    int *count = (int*)malloc(numranks*sizeof(int));
    int *start = (int*)malloc(numranks*sizeof(int));
    int *end = (int*)malloc(numranks*sizeof(int));

    srand(time(NULL));

    //fills and outputs the input array
    //also initalizes the match array to 0
    if(rank == 0){
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
    }

    //Broadcasts the input and match arrays
    MPI_Bcast(list, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(match, max, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        //Create and fills array of the number of variables in each rank
        for(int i = 0; i < numranks; i++){
            count[i] = n / numranks;
        }

        if(n % numranks != 0){
            int rem = n / numranks;
            for(int i = 0; i < rem; i ++){
                count[i] = count[i] + 1;
            }
        }

/*
        for(int i = 0; i < numranks; i++){
            printf("Count @ Rank %d = %d\n", i, count[i]);
        }
*/

        //Creates and fills array of start values for each rank
        start[0] = 0;
        for(int i = 1; i < numranks; i++){
            start[i] = start[i-1] + count[i-1];
        }

/*    
        for(int i = 0; i < numranks; i++){
            printf("Start @ Rank %d = %d\n", i, start[i]);
        }
*/

        //Creates and fills array end values for each rank
        end[numranks-1] = 0;
        for(int i = 0;i < numranks; i++){
            end[i] = start[i] + count[i] -1;
        }

/*
        for(int i = 0; i < numranks; i++){
            printf("End @ Rank %d = %d\n", i, end[i]);
        }
*/

    }

    //Broadcats the start and end values for each rank
    MPI_Bcast(start, numranks, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(end, numranks, MPI_INT, 0, MPI_COMM_WORLD);

    //Creates variables for the start and end values
    int mystart = start[rank];
    int myend = end[rank];

    //Counts the number of times each value appears in the list
    for(int i = mystart; i <= myend; i++){
        num = list[i];
        match[num]++;
    }

    //Combines work from all ranks
    for(int i = 0; i <= max; i++){
        MPI_Allreduce(&match[i], &result[i], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    }

    //Outputs the results
   if(rank == numranks-1){
/*
       printf("Count Array = [");
       for(int i = 0; i <= max; i++){
            if(i != n){
                printf("%d,", result[i]);
            }
            else{
                printf("%d", result[i]);
            }
       }
       printf("]\n");
*/
       counter = 0;
        printf("Output Array = [");
        for(int i = 0; i <= max; i++){
            while(result[i]>0){
                if(counter != n-1){
                    printf("%d,", i);
                    counter++;

                }
                else{
                    printf("%d", i);
                }
                result[i]--;
            }
        }
        printf("]\n");
    }

    double endTime = MPI_Wtime();
    double runtime = endTime - startTime;

    printf("Rank %d took %f\n", rank, runtime);

    MPI_Allreduce(MPI_IN_PLACE, &runtime, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if(rank == numranks - 1){
        printf("Total runtime: %f\n", runtime);
    }

    MPI_Finalize();
    return 0;
}
