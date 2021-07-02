#include <stdio.h>

void swap ( int* a , int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int  partison ( int arr [], int low , int high )
{
    int pivot = arr [high];
    int result_index = low - 1 ;

    for ( int less_than_pivot = low ; less_than_pivot <= high -1 ; less_than_pivot ++)
    {
        if ( arr [ less_than_pivot ] < pivot )
        {
            result_index ++;
            swap ( &arr[ result_index ], &arr[ less_than_pivot ] );
        }
    }

    int pi = result_index + 1 ;
    swap ( &arr[ high ] , &arr [ pi ] );

    return pi;

}

void quickSort ( int arr[], int low , int high )
{
    if ( low < high )
    {
        int pi = partison ( arr, low, high );

        quickSort ( arr, low, pi -1 );
        quickSort ( arr, pi + 1 , high );
    }
    else
    {
        printf ("Since low %d is greater than high %d \n", low, high );
    }
}





void printArray ( int arr [], int size)
{
    for ( int i = 0; i <= size; i++)
    {
        printf ("%d  ", arr[i]);
    }
    printf ("\n");
}

int main ()
{
    int arr [] = { 4,1, 17, 18, 15, 10, 7, 6 , 3};
    int size = sizeof (arr)/ sizeof (arr[0]);
    printArray ( arr, size -1 );

    quickSort ( arr, 0, size -1 );

    printArray ( arr, size -1 );
}