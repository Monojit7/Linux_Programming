// Prim's Algorithm in C++

#include <cstring>
#include <iostream>
using namespace std;

#define INF 9999999

// number of vertices in grapj
#define V 5

// create a 2d array of size 5x5
//for adjacency matrix to represent graph

int G[V][V] = {
  {0, 9, 75, 0, 0},
  {9, 0, 95, 19, 42},
  {75, 95, 0, 51, 66},
  {0, 19, 51, 0, 31},
  {0, 42, 66, 31, 0}};

int main() {

  int no_edge = 0;
  int x = 0;
  int y = 0;

  bool selected [V];
  selected[0] = true;

  printf ("Edge --> weight :  \n");
  while ( no_edge < ( V - 1 ) )
  {
    int x = 0;
    int y = 0;
    int min = INF;

    for ( int i = 0; i < V; i++ )
    {
      if ( selected [i])
      {
        for ( int j = 0; j < V; j++ )
        {
          if ( !selected[j] && G[i] [j])
          {
              if ( min > G[i][j])
              {
                min = G[i][j];
                x = i;
                y = j;
              }
          }
        }
      }
    }

    printf (" %d --> %d and weight : %d \n", x,y, min);
    selected[y] = true;
    no_edge++;


  }
}