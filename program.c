#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    int age;
} User;

// Function prototypes
void createUser();
void readUser();
void updateUser();
void deleteUser();

int main()
{
    bool turn = true;
    while (turn)
    {
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                createUser();
                break;
            case 2:
                readUser();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                turn = false;
                break;
            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }
    }

    return 0;
}

void createUser()
{
    User user;

    printf("Enter User ID: \n");
    scanf("%d",&user.id);
    getchar();

    printf("Enter User Name: \n");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strlen(user.name)-1]='\0';

    printf("Enter User Age: ");
    scanf("%d",&user.age);

    FILE *file = fopen("users.txt", "a");
    if (!file)
    {
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "User ID: %d, User Name: %s, User Age: %d",user.id,user.name,user.age);
    fclose(file);

    printf("User added successfully");
}

void readUser()
{

}


void updateUser()
{

}


void deleteUser()
{

}
