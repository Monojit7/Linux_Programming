#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
 
#define N 3
 
// In-place rotate it by 90 degrees in a clockwise direction
void rotate(int mat[N][N])
{
    // Transpose the matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++) {
            swap(mat[i][j], mat[j][i]);
        }
    }
 
    // swap columns
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N/2; j++) {
            swap(mat[i][j], mat[i][N - j - 1]);
        }
    }
}
 
void printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++) {
            cout << setw(3) << mat[i][j] << " ";
        }
        cout << endl;
    }
}
 
int main()
{
    int mat[N][N] =
    {
        { 1, 2, 3 },
        { 4,5,6},
        { 7,8,9 }
    };
 
    rotate(mat);
    printMatrix(mat);
 
    return 0;
}