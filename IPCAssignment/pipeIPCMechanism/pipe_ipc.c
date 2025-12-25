#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void sort(int arr[], int noOfElements)
{
    for(int i = 0; i < noOfElements - 1; i++)
    {
        for(int j = i + 1; j < noOfElements; j++)
        {
            if(arr[i] > arr[j])
            {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }
}

int main()
{
    int fd[2];
    pipe(fd);

    int noOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &noOfElements);

    int *arr = (int *)malloc(noOfElements * sizeof(int));

    printf("Enter array elements:\n");
    for(int i = 0; i < noOfElements; i++)
    {
        scanf("%d", &arr[i]);
    }


    printf("Before Sorting: ");
    for(int i = 0; i < noOfElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    if(fork() == 0)
    {
        read(fd[0], &noOfElements, sizeof(int));

        int *childArr = (int *)malloc(noOfElements * sizeof(int));
        read(fd[0], childArr, noOfElements * sizeof(int));

        sort(childArr, noOfElements);

        write(fd[1], &noOfElements, sizeof(int));
        write(fd[1], childArr, noOfElements * sizeof(int));

        free(childArr);
        exit(0);
    }
    write(fd[1], &noOfElements, sizeof(int));
    write(fd[1], arr, noOfElements * sizeof(int));

    wait(NULL);

    read(fd[0], &noOfElements, sizeof(int));
    read(fd[0], arr, noOfElements * sizeof(int));

    printf("After Sorting: ");
    for(int i = 0; i <noOfElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
