#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

void mergeSort(int[], int, int);
void merge(int[], int, int, int);

int main(int argc, char **argv)
{
    //Initialize MPI Variables
    MPI_Init(&argc, &argv);
    int rank, numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;
    int N = 1500000;
    int workers = numranks - 1;

    if (numranks == 1)
    {
        printf("Master-Worker failure, more than 1 nodes required to run this program");
        return 1;
        MPI_Finalize();
    }

    //Create memory for arrays to be used for before, during, and after sort
    int *original = (int *)malloc(sizeof(int) * N);

    //Create memory for MPI related arrays
    int *sendcounts = (int *)malloc(sizeof(int) * numranks);
    int *displacement = (int *)malloc(sizeof(int) * numranks);
    int *start = (int *)malloc(sizeof(int) * numranks);
    int *end = (int *)malloc(sizeof(int) * numranks);

    double *startTime = (double *)malloc(sizeof(double) * numranks);
    double *endTime = (double *)malloc(sizeof(double) * numranks);

    startTime[rank] = MPI_Wtime();
    if (rank == 0)
    { //Initiate orignial array and send to workers
        srand(time(0));
        for (int i = 0; i < N; i++)
        {
            original[i] = rand() % 1000000;
        }
        printf("Original Array: [");
        for (int i = 0; i < N; i++)
        {
            if (i == N - 1)
            {
                printf("%d]\n", original[i]);
            }
            else
            {
                printf("%d, ", original[i]);
            }
        }

        int remainder = N % workers;
        for (int i = 1; i < numranks; i++)
        { //Calcuate sendcounts to help determine respective starts and ends
            sendcounts[i] = N / workers;
            if (remainder > 0)
            {
                sendcounts[i]++;
                remainder--;
            }
        }

        start[1] = 0;
        end[1] = sendcounts[1];

        for (int i = 2; i < numranks; i++)
        { //Assign start and end values to be used by each worker rank
            end[i] = end[i - 1] + sendcounts[i];
            start[i] = end[i - 1] + 1;
        }

        for (int i = 1; i < numranks; i++)
        { //Send array values to worker rank and recieve results
            MPI_Send(start, numranks, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(end, numranks, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(original, N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Recv(original, N, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
        }
        printf("Sorted Array: [");
        for (int i = 0; i < N; i++)
        {
            if (i == N - 1)
            {
                printf("%d]\n", original[i]);
            }
            else
            {
                printf("%d, ", original[i]);
            }
        }
        endTime[rank] = MPI_Wtime();
    }
    if (rank != 0)
    { //Workers recieve respective arrays and complete merge sort
        MPI_Recv(start, numranks, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(end, numranks, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(original, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        mergeSort(original, start[rank], end[rank]);
        MPI_Send(original, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        endTime[rank] = MPI_Wtime();
    }
    MPI_Finalize();
    

    for (int i = 0; i < numranks; i++)
    {
        printf("Total time for Rank[%d] is: %.2f\n", i, endTime[i] - startTime[i]);
    }
    return 0;
}

//Follows GeekforGeeks MergeSort Implementation (https://www.geeksforgeeks.org/merge-sort/)
void merge(int arr[], int start, int middle, int end)
{
    int i, j, k;
    int n1 = middle - start + 1;
    int n2 = end - middle;

    /* create temp arrays */
    int temp1[n1], temp2[n2];

    // Copy data to temp arrays
    for (i = 0; i < n1; i++)
        temp1[i] = arr[start + i];
    for (j = 0; j < n2; j++)
        temp2[j] = arr[middle + j + 1];

    /* Merge the temp arrays back into arr[start..end]*/
    i = 0;     // Initial index of first subarray
    j = 0;     // Initial index of second subarray
    k = start; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (temp1[i] <= temp2[j])
        {
            arr[k] = temp1[i];
            i++;
        }
        else
        {
            arr[k] = temp2[j];
            j++;
        }
        k++;
    }

    //Copy remaining elements for Sub-Array 1
    while (i < n1)
    {
        arr[k] = temp1[i];
        i++;
        k++;
    }

    //Copy remaining elements for Sub-Array 2
    while (j < n2)
    {
        arr[k] = temp2[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int start, int end)
{
    if (start < end)
    {
        int middle = start + (end - start) / 2;
        mergeSort(arr, start, middle);
        mergeSort(arr, middle + 1, end);
        merge(arr, start, middle, end);
    }
}