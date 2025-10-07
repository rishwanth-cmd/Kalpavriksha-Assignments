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
        //validating user input
        if(scanf("%d", &choice)!=1)
        {
            printf("Please enter a valid option.\n");
            while(getchar() != '\n');
            continue;
        }


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

    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        file = fopen("users.txt", "w");
        fclose(file);
        file = fopen("users.txt", "r");
    }

    printf("Enter User ID: \n");
    scanf("%d",&user.id);
    getchar();

    //checking for duplicate id's
    User temp;
    int found=0;
    while(fscanf(file, "User ID: %d, User Name: %49[^,], User Age: %d\n", &temp.id, temp.name, &temp.age) == 3)
    {
        if(temp.id==user.id)
        {
            found=1;
            break;
        }
    }
    fclose(file);
    if(found)
    {
        printf("User ID: %d already exists. Please enter a new ID.\n",user.id);
        return;
    }

    printf("Enter User Name: \n");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strlen(user.name)-1]='\0';

    printf("Enter User Age: ");
    scanf("%d",&user.age);

    file = fopen("users.txt", "a");


    fprintf(file, "User ID: %d, User Name: %s, User Age: %d\n",user.id,user.name,user.age);
    fclose(file);

    printf("User added successfully\n");
}

void readUser()
{
    FILE *file = fopen("users.txt","r");

    if(file==NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int id;
    char name[50];
    int age;

    while(fscanf(file, "User ID: %d, User Name: %49[^,], User Age: %d\n", &id, name, &age)==3)
    {
        printf("User ID: %d, User Name: %s, User Age: %d\n",id,name,age);
    }

    fclose(file);
}


void updateUser()
{
    User user;
    int id;
    char newName[50];
    int newAge;
    int found=0;
    printf("Enter the ID to update: ");
    scanf("%d",&id);
    getchar();

    FILE *file = fopen("users.txt","r");
    FILE *temp = fopen("newFile.txt","w");
    if(file==NULL || temp==NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    while(fscanf(file, "User ID: %d, User Name: %49[^,], User Age: %d\n", &user.id, user.name, &user.age)==3)
    {
        if(id==user.id)
        {
            printf("Enter new User Name: ");
            fgets(newName, sizeof(newName), stdin);
            newName[strlen(newName)-1]='\0';

            printf("Enter new User Age: ");
            scanf("%d",&newAge);

            fprintf(temp, "User ID: %d, User Name: %s, User Age: %d\n", user.id, newName, newAge);
            found=1;
        }
        else
        {
            fprintf(temp, "User ID: %d, User Name: %s, User Age: %d\n", user.id, user.name, user.age);

        }
    }
    fclose(file);
    fclose(temp);
    remove("users.txt");
    rename("newFile.txt","users.txt");
    if(found)
    {
        printf("User ID: %d updated successfully.\n",id);
    }
    else
    {
        printf("No user found with User ID: %d\n",id);
    }

}


void deleteUser()
{
    User user;
    int id;
    printf("Enter the User ID to delete: ");
    scanf("%d",&id);
    int found=0;

    FILE *file = fopen("users.txt","r");
    FILE *temp = fopen("newFile.txt","w");

    if(file==NULL || temp==NULL)
    {
        printf("Error opening in file.\n");
        return;
    }

    while(fscanf(file, "User ID: %d, User Name: %49[^,], User Age: %d\n", &user.id, user.name, &user.age)==3)
    {
        if(user.id==id)
        {
            found=1;
            continue;
        }
        fprintf(temp, "User ID: %d, User Name: %s, User Age: %d\n", user.id, user.name, user.age);
    }
    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("newFile.txt", "users.txt");

    if(found==0)
    {
        printf("No user was found with the ID: %d\n",id);
    }
    else
    {
        printf("User ID: %d was deleted successfully.\n",id);
    }
}
