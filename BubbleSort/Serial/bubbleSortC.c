#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    //Initalize and Dynamically Allocate Original Array
    int N = 100;
    int *arr = (int *)malloc(sizeof(int) * N);
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 100;
    }

    //Bubble Sort Implementation from textbook
    for (int list_length = N; list_length >= 2; list_length--)
    {
        for (int i = 0; i < list_length - 1; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        printf("Array[%d]: %d\n", i, arr[i]);
    }

    free(arr);

    return 0;
}