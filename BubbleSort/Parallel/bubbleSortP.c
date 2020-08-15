#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    //Initalize and Dynamically Allocate Original Array
    int N = 10;
    int i, temp, list_length;
    int *arr = (int *)malloc(sizeof(int) * N);
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 100;
    }
    printf("Original Array: [");
    for (int i = 0; i < N; i++) 
    {
        if(i == N-1){
            printf("%d]\n",arr[i]);
        }else{
            printf("%d, ",arr[i]);
        }
    }

#pragma omp parallel default(shared) private(i, temp, list_length)//Being Parallel Region
    {
        for (list_length = N; list_length >= 2; list_length--)
        {
#pragma omp parallel for //Seperates for per each thread
            for (i = 0; i < list_length - 1; i++)
            {//Perform Bubble Sort
                if (arr[i] > arr[i + 1])
                {
                    temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        } //end of parallel for
    }//end parallel region
    printf("Sorted Array: [");
    for (int i = 0; i < N; i++) 
    {//Print Sorted array
        if(i == N-1){
            printf("%d]\n",arr[i]);
        }else{
            printf("%d, ",arr[i]);
        }
    }

    free(arr);//free aray

    return 0;//terminate program
}