#include <stdio.h>

void findMinMaxIsolatedVertx ( int v, int e )
{
    int niso = v - 2*e;

    if ( niso > 0 )
    {
        printf ( "minimum isolated vertex is %d \n", niso);
    }
    else
    {
        printf ("minimum isolated vertex is 0 \n" );
    }


   int i = 0;
    for ( i = 1; i<= v; i++ )
    {
        // n*(n-1)/2 is total edges or max edges
        if ( i*(i-1)/2 >= e )
        break;
    }

    int max_iso_vertex = v - i;
    printf ("maximum number of isolated vertex is %d \n", max_iso_vertex );
}

int main ()
{
    int v = 5;
    int e = 2;

    findMinMaxIsolatedVertx ( v , e );
    return 0;
    return 0;

}