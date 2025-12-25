#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    int noOfElements;
    FILE *fp;

    printf("Enter number of elements: ");
    scanf("%d", &noOfElements);

    int *arr = (int *)malloc(noOfElements * sizeof(int));
    if(arr == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

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

    fp = fopen("data.txt", "w");
    for(int i = 0; i < noOfElements; i++)
    {
        fprintf(fp, "%d ", arr[i]);
    }
    fclose(fp);

    if(fork() == 0)
    {
        fp = fopen("data.txt", "r");
        for(int i = 0; i < noOfElements; i++)
        {
            fscanf(fp, "%d", &arr[i]);
        }
        fclose(fp);

        sort(arr, noOfElements);

        fp = fopen("data.txt", "w");
        for(int i = 0; i < noOfElements; i++)
        {
            fprintf(fp, "%d ", arr[i]);
        }
        fclose(fp);
        exit(0);
    }
    wait(NULL);

    fp = fopen("data.txt", "r");
    for(int i = 0; i < noOfElements; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }
    fclose(fp);

    printf("After Sorting: ");
    for(int i = 0; i < noOfElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
