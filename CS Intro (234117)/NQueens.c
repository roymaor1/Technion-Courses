/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*=========================================================================
  Constants and definitions:
==========================================================================*/

#define N 4

/*-------------------------------------------------------------------------
object is to print one solution for the NQueens Problem.
 -------------------------------------------------------------------------*/
 int Abs (int a, int b)
    {
        if(a-b>0)
            return a-b;
        return b-a;
    }
void print_array (int board[N][N])
    {
        for(int i=0; i<N; i++)
            {
                for(int j=0; j<N; j++)
                    printf("%d ",board[i][j]);
                printf("\n");
            }
    }
bool UnderAttack (int r1, int c1, int r2, int c2)
    {
        if(r1==r2 || c1==c2 || Abs(r1,r2)==Abs(c1,c2))
            return true;
        return false;
    }
bool IsSpotSafe (int board[N][N], int row, int col) //recieves board and spot and tells if safe
    {
        for(int i=0; i<N; i++)
            {
                for(int j=0; j<N; j++)
                    {
                        if(i==row && j==col) continue;
                        if(board[i][j]==1 && UnderAttack(i,j,row,col)) return false;
                    }
            }
        return true;
    }
void NQueens_Internal (int board[N][N], int row)
    {
        printf("Im here");
        if(row==N)
            {
                print_array(board);
                return;
            }
        for(int i=0; i<N; i++) //move along columns of row
            {
                board[row][i]=1;
                if(IsSpotSafe(board, row, i))
                    NQueens_Internal(board, row+1);
                board[row][i]=0; //reset
            }
    }
void NQueens (int board[N][N])
    {
        printf("Im here");
        NQueens_Internal(board,0);
    }
int main()
{
  int board[N][N]={{0}};
  NQueens(board);
  return 0;
}
