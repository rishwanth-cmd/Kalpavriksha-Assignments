#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100

struct msg
{
    long type;
    int noOfElements;
    int arr[MAX];
};

void sort(int arr[], int noOfElements)
{
    for(int i = 0; i < noOfElements; i++)
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
    int msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    struct msg m;

    printf("Enter number of elements: ");
    scanf("%d", &m.noOfElements);

    printf("Enter array elements:\n");
    for(int i = 0; i < m.noOfElements; i++)
    {
        scanf("%d", &m.arr[i]);
    }

    printf("Before Sorting: ");
    for(int i = 0; i < m.noOfElements; i++)
    {
        printf("%d ", m.arr[i]);
    }
    printf("\n");

    if(fork() == 0)
    {
        msgrcv(msgid, &m, sizeof(m) - sizeof(long), 1, 0);

        sort(m.arr, m.noOfElements);

        m.type = 2;
        msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0);
        exit(0);
    }

    m.type = 1;
    msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0);

    wait(NULL);

    msgrcv(msgid, &m, sizeof(m) - sizeof(long), 2, 0);

    printf("After Sorting: ");
    for(int i = 0; i < m.noOfElements; i++)
    {
        printf("%d ", m.arr[i]);
    }
    printf("\n");

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
