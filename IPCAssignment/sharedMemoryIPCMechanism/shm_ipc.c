#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void sort(int *arr, int noOfElements)
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

    printf("Enter number of elements: ");
    scanf("%d", &noOfElements);

    int shmid = shmget(IPC_PRIVATE, (noOfElements + 1) * sizeof(int), 0666 | IPC_CREAT);

    int *shm = (int *)shmat(shmid, NULL, 0);
    shm[0] = noOfElements;

    printf("Enter array elements:\n");
    for(int i = 0; i < noOfElements; i++)
    {
        scanf("%d", &shm[i + 1]);
    }

    printf("Before Sorting: ");
    for(int i = 0; i < noOfElements; i++)
    {
        printf("%d ", shm[i + 1]);
    }
    printf("\n");

    if(fork() == 0)
    {
        int n = shm[0];
        sort(&shm[1], n);
        exit(0);
    }
    wait(NULL);

    printf("After Sorting: ");
    for (int i = 0; i < noOfElements; i++)
    {
        printf("%d ", shm[i + 1]);
    }
    printf("\n");

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
