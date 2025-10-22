#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rotate90Degree(int **matrix, int matrixSize);
void displayMatrix(int **matrix, int matrixSize);
void applySmoothing(int **matrix, int matrixSize);

int main()
{
    int matrixSize;
    printf("Enter the size of the matrix between 2 - 10: \n");
    scanf("%d",&matrixSize);
    while(matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid size. Enter again: \n");
        scanf("%d",&matrixSize);
    }

    srand(time(NULL));

    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for(int i = 0; i < matrixSize; i++)
    {
        *(matrix + i) = (int *)malloc(matrixSize * sizeof(int));
    }

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }

    displayMatrix(matrix, matrixSize);
    rotate90Degree(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
    applySmoothing(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
}


void rotate90Degree(int **matrix, int matrixSize)
{
    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = i; j < matrixSize; j++)
        {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + j) + i);
            *(*(matrix + j) + i) = temp;
            
        }
    }

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize/2; j++)
        {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + i) + (matrixSize - 1 - j));
            *(*(matrix + i) + (matrixSize - 1 - j)) = temp;
        }
    }
}

void displayMatrix(int **matrix, int matrixSize)
{
    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
    printf("\n");
}

void applySmoothing(int **matrix, int matrixSize)
{
    int *tempMatrix=(int*)malloc(matrixSize * matrixSize * sizeof(int));

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            *(tempMatrix + i * matrixSize + j) = *(*(matrix + i) + j);
        }
    }

    for(int row = 0; row < matrixSize; row++)
    {
        for(int column = 0; column < matrixSize; column++)
        {
            int sum = 0;
            int count = 0;

            for(int i = row - 1; i <= row + 1; i++)
            {
                for(int j = column - 1; j <= column + 1; j++)
                {
                    if(i >= 0 && i < matrixSize && j >= 0 && j < matrixSize)
                    {
                        sum += *(tempMatrix + i * matrixSize + j);
                        count++;
                    }
                }
            }
            *(*(matrix + row) + column) = sum / count; 
        }
    }
    free(tempMatrix);
}