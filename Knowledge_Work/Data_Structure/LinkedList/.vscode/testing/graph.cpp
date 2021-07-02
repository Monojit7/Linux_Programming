// Represent a graph into 2D adajacent matrix

#include <stdio.h>

#define N 5
#define EDGES 5


void createAdj ( int arr[] [2], int adj [] [N+1], int M )
{

    for ( int i = 0 ;i < N  + 1 ; i++)
    {
        for ( int j = 0; j < N + 1 ; j++)
        {
            adj [i] [j] = 0;
        }
    }

    for ( int i = 0; i < M ; i++)
    {
        int x = arr [i] [0];
        int y = arr [i] [1];

        adj [x] [y] = 1;
        adj [y] [x] = 1;
    }

}

void printGraph ( int adj[] [N+1])
{
    printf ("Adjacency matrix is -> \n");
    for ( int i = 0 ; i < N+1 ; i++)
    {
        for ( int j = 0; j < N + 1 ; j++)
        {
            printf (" %d  ", adj [i] [j] ) ;
        }
        printf ("\n");
    }
}

int main ()
{
    
    int arr[] [2] = { {1,2}, {2,3}, {4,5}, {1,5} };

    int M = sizeof (arr)/ sizeof ( arr[0]);

    int adj [ N+1 ] [ N+1 ];

    createAdj ( arr , adj, M );

    printGraph ( adj );

}