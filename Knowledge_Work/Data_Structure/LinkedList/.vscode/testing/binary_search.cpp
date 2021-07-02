#include <stdio.h>


int binary_search ( int a[], int data, int low, int high )
{
   if ( low <= high )
   {
       int mid = ( low + high ) / 2;

       if ( a [ mid ] ==  data )
       {
          return mid;
       }
       else if ( data < a [ mid ])
       {
           return binary_search ( a, data, 0 , mid -1  );
       }
       else if ( data > a [ mid ])
       {
           return binary_search ( a , data, mid + 1  , high );
       }
   }
  
 
}

int main ()
{
    int arr [] = { 1, 13, 15, 16, 23, 29, 31};

    int size = sizeof (arr)/ sizeof (arr[0]);

    printf ("the size if the array is %d\n", size );

    printf (" position of the element in the array %d \n", binary_search ( arr , 22, 0, size - 1 ) );

   // int position = binary_search ( arr, 17);

   // printArray ( arr, size -1 );
}
