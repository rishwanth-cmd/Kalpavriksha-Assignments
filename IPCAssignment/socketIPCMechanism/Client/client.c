#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    int choice, amount, result;

    while(1)
    {
        printf("\n--- ATM MENU ---\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        write(sock, &choice, sizeof(choice));

        if(choice == 1 || choice == 2)
        {
            printf("Enter amount: ");
            scanf("%d", &amount);
            write(sock, &amount, sizeof(amount));
        }
        else if(choice == 4)
        {
            printf("Exiting...\n");
            break;
        }

        read(sock, &result, sizeof(result));
        printf("Current Balance: %d\n", result);
    }

    close(sock);
    return 0;
}
