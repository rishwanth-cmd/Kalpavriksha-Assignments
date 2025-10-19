#include <stdio.h>
#include <stdlib.h>

void rotate90Degree(int **matrix, int n);
void displayMatrix(int **matrix, int n);

int main()
{
    int n;
    printf("Enter the size of the matrix: \n");
    scanf("%d",&n);

    int **matrix = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++)
    {
        *(matrix + i) = (int *)malloc(n * sizeof(int));
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }

    displayMatrix(matrix, n);
    rotate90Degree(matrix, n);
    displayMatrix(matrix, n);

}


void rotate90Degree(int **matrix, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + j) + i);
            *(*(matrix + j) + i) = temp;
            
        }
    }
}

void displayMatrix(int **matrix, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
    printf("\n");
}