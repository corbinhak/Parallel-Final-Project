#include <stdio.h>
#include <stdlib.h>

void mergeSort(int[], int, int);
void merge(int[], int, int, int);

int main(int argc, char **argv)
{
    //Number of elements
    int N = 100;
    //Initalize Array to be sorted
    int *arr = (int *)malloc(sizeof(int)*N); //allocate 100 ints
    for (int i =0; i < N; i++) {
        arr[i] = rand() % 2147483647;
    }
    mergeSort(arr, 0, N);
    for (int i =0; i<100; i++) {
        printf("Array[%d] = %d\n", i, arr[i]);
    }
    free(arr);
    return 0;
} 

void merge(int arr[], int start, int middle, int end)
{
    int i, j, k;
    int n1 = middle - start + 1;
    int n2 =  end - middle;

    /* create temp arrays */
    int temp1[n1], temp2[n2];

    // Copy data to temp arrays 
    for (i = 0; i < n1; i++)
        temp1[i] = arr[start + i];
    for (j = 0; j < n2; j++)
        temp2[j] = arr[middle + j + 1];

    /* Merge the temp arrays back into arr[start..end]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
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
        int middle = start+(end-start)/2;
        mergeSort(arr, start, middle);
        mergeSort(arr, middle+1, end);
        merge(arr, start, middle, end);
    }
}