#include <stdio.h>
#include <stdlib.h>

void rotate90Degree(int **matrix, int n);
void displayMatrix(int **matrix, int n);
void applySmoothing(int **matrix, int n);

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
    applySmoothing(matrix, n);
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

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n/2; j++)
        {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + i) + (n - 1 - j));
            *(*(matrix + i) + (n - 1 - j)) = temp;
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

void applySmoothing(int **matrix, int n)
{
    int *tempMatrix=(int*)malloc(n *n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(tempMatrix + i * n + j) = *(*(matrix + i) + j);
        }
    }

    for(int row = 0; row < n; row++)
    {
        for(int column = 0; column < n; column++)
        {
            int sum = 0;
            int count = 0;

            for(int i = row - 1; i <= row + 1; i++)
            {
                for(int j = column - 1; j <= column + 1; j++)
                {
                    if(i >= 0 && i < n && j >= 0 && j < n)
                    {
                        sum += *(tempMatrix + i * n + j);
                        count++;
                    }
                }
            }
            *(*(matrix + row) + column) = sum / count; 
        }
    }
    free(tempMatrix);
}