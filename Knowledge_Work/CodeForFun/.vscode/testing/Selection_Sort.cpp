#include <stdio.h>

void printArray ( int arr [], int size )
{
    for ( int i = 0; i <=size ; i++)
    {
        printf (" %d ", arr[i]);
    }

    printf ("\n");
}

void selectionSort ( int a[], int n )
{
    for ( int i = 0 ; i < n - 1 ; i++ )
    {
        int min = i;
        for ( int j = i+1 ; j < n ; j++ )
        {
            if ( a[j] < a[min])
            {
                min = j;
            }
        }

        if ( min != i )
        {
            int temp = a[i];
            a[i] = a[min];
            a[min ] = temp;
        }
    }
}


void insertion_sort ( int a[], int n)
{
    
}

int main()
{
    int arr[] = { 5, 3, 8, 2, 1};

    int arr_i [] = { 34, 3, 23, 11, 25, 21};
    int num_i = sizeof (arr_i)/ sizeof (arr_i[0]);
    int num = sizeof (arr)/ sizeof (arr[0]);

    printArray ( arr_i, num_i - 1 );

    insertion_sort ( arr_i, num_i );

   printArray ( arr_i, num_i -1 );
    

}