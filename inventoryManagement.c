#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MINIMUM_NO_OF_PRODUCTS 1
#define MAXIMUM_NO_OF_PRODUCTS 100
#define MINIMUM_ID 1
#define MAXIMUM_ID 10000
#define MAXIMUM_LENGTH 50
#define MINIMUM_PRICE 1
#define MAXIMUM_PRICE 100000
#define MINIMUM_QUANTITY 1
#define MAXIMUM_QUANTITY 1000000

typedef struct
{
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

void addProduct(Product **product, int *currentProductCapacity, int *totalProductCapacity);
void viewProduct(Product *product, int currentProductCapacity);
void updateQuantity(Product *product, int currentProductCapacity);
void searchProductByID(Product *product, int currentProductCapacity);
void searchProductByName(Product *product, int currentProductCapacity);
void searchProductByPrice(Product *product, int currentProductCapacity);
void deleteProduct(Product **product, int *currentProductCapacity);

int main()
{
    Product *product;
    int noOfProducts;
    do
    {
        printf("Enter the initial number of products (1 - 100): ");
        scanf("%d", &noOfProducts);
        if(noOfProducts < MINIMUM_NO_OF_PRODUCTS || noOfProducts > MAXIMUM_NO_OF_PRODUCTS)
        {
            printf("Invalid size. Please try again.\n");
        }

    } while(noOfProducts < MINIMUM_NO_OF_PRODUCTS || noOfProducts > MAXIMUM_NO_OF_PRODUCTS);

    product = (Product *)calloc(noOfProducts, sizeof(Product));

    if (product == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }


    for(int i = 0; i < noOfProducts; i++)
    {
        printf("Enter the details of product %d:\n", i+1);
        printf("Enter Product ID: ");
        scanf(" %d", &product[i].id);
        if(product[i].id < MINIMUM_ID || product[i].id > MAXIMUM_ID)
        {
            printf("Please enter the product ID range between %d - %d.\n", MINIMUM_ID, MAXIMUM_ID);
            i--;
            continue;
        }
        printf("Enter Product Name: ");
        scanf(" %50s", product[i].name);
        if(strlen(product[i].name) > MAXIMUM_LENGTH)
        {
            printf("Please enter product name length less than %d.\n",MAXIMUM_LENGTH);
            i--;
            continue;
        }
        printf("Enter Product Price: ");
        scanf(" %f", &product[i].price);
        if(product[i].price < MINIMUM_PRICE || product[i].price > MAXIMUM_PRICE)
        {
            printf("Please enter the product price range between %d - %d.\n", MINIMUM_PRICE, MAXIMUM_PRICE);
            i--;
            continue;
        }
        printf("Enter Product Quantity: ");
        scanf(" %d", &product[i].quantity);
        if(product[i].quantity < MINIMUM_QUANTITY || product[i].quantity > MAXIMUM_QUANTITY)
        {
            printf("Please enter the product price range between %d - %d.\n", MINIMUM_QUANTITY, MAXIMUM_QUANTITY);
            i--;
            continue;
        }
    }

    bool turn = true;
    int currentProductCapacity = noOfProducts;
    int totalProductCapacity = noOfProducts;
    

    while(turn)
    {
        printf("========= INVENTORY MENU =========\n");
        printf("1. Add a new product\n");
        printf("2. View all products\n");
        printf("3. Update the quantity of a product\n");
        printf("4. Search for a product by ID\n");
        printf("5. Search for a product by name\n");
        printf("6. Search for product by price range\n");
        printf("7. Delete a product by ID\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf(" %d",&choice);

        switch(choice)
        {
            case 1:
                addProduct(&product, &currentProductCapacity, &totalProductCapacity);
                break;

            case 2:
                viewProduct(product, currentProductCapacity);
                break;
        
            case 3:
                updateQuantity(product, currentProductCapacity);
                break;
        
            case 4:
                searchProductByID(product, currentProductCapacity);
                break;
            
            case 5:
                searchProductByName(product, currentProductCapacity);
                break;
        
            case 6:
                searchProductByPrice(product, currentProductCapacity);
                break;
        
            case 7:
                deleteProduct(&product, &currentProductCapacity);
                break;
            
            case 8:
                turn = false;
                free(product);
                product = NULL;
                printf("Freed memory!\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");

        }

    }


}



void addProduct(Product **product, int *currentProductCapacity, int *totalProductCapacity)
{
    if(*currentProductCapacity == *totalProductCapacity)
    {
        *totalProductCapacity += 1;
        *product = (Product *)realloc(*product, (*totalProductCapacity) * sizeof(Product));
        if(*product == NULL)
        {
            printf("ReAllocation failed.\n");
            return;
        }
    }

    printf("Enter details for new product: \n");
    printf("Enter Product ID: ");
    scanf(" %d", &(*product)[*currentProductCapacity].id);
    if((*product)[*currentProductCapacity].id < MINIMUM_ID || (*product)[*currentProductCapacity].id > MAXIMUM_ID)
    {
        printf("Please enter the product ID range between %d - %d.\n", MINIMUM_ID, MAXIMUM_ID);
        return;
    }
    printf("Enter Product Name: ");
    scanf(" %50s", (*product)[*currentProductCapacity].name);
    if(strlen((*product)[*currentProductCapacity].name) > MAXIMUM_LENGTH)
    {
        printf("Please enter product name length less than %d.\n", MAXIMUM_LENGTH);
    }
    printf("Enter Product Price: ");
    scanf(" %f", &(*product)[*currentProductCapacity].price);
    if((*product)[*currentProductCapacity].price < MINIMUM_PRICE || (*product)[*currentProductCapacity].price > MAXIMUM_PRICE)
    {
        printf("Please enter the product price range between %d - %d.\n", MINIMUM_ID, MAXIMUM_ID);
    }
    printf("Enter Product Quantity: ");
    scanf(" %d", &(*product)[*currentProductCapacity].quantity);
    if((*product)[*currentProductCapacity].quantity < MINIMUM_QUANTITY || (*product)[*currentProductCapacity].quantity > MAXIMUM_QUANTITY)
    {
        printf("Please enter the product price range between %d - %d.\n", MINIMUM_QUANTITY, MAXIMUM_QUANTITY);
    }

    (*currentProductCapacity)++;

    printf("Product Added Successfully!\n");

}

void viewProduct(Product *product, int currentProductCapacity)
{
     for(int i = 0; i < currentProductCapacity; i++)
     {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", product[i].id, product[i].name, product[i].price, product[i].quantity);
    }
}


void updateQuantity(Product *product, int currentProductCapacity)
{
    int id;
    printf("Enter the product ID that need to be updated: ");
    scanf("%d", &id);
    int newQuantity;
    bool found = false;

    for(int i = 0; i < currentProductCapacity; i++)
    {
        if(product[i].id == id)
        {
            printf("Enter new quantity: ");
            scanf("%d", &newQuantity);
            product[i].quantity = newQuantity;
            found = true;
            printf("Quantity updated successfully.\n");
            break;
        }

    }
    if(!found)
    {
        printf("Product with ID %d not found.\n", id);
    }
}

void searchProductByID(Product *product, int currentProductCapacity)
{
    int id;
    bool found = false;
    printf("Enter the product ID that need to be searched: ");
    scanf("%d", &id);
    for(int i = 0; i < currentProductCapacity; i++)
    {
        if(product[i].id == id)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", product[i].id, product[i].name, product[i].price, product[i].quantity);
            found = true;
            break;

        }
    }
    if(!found)
    {
        printf("Product with ID %d not found.\n", id);
    }

}


void searchProductByName(Product *product, int currentProductCapacity)
{
    char name[50];
    bool found = false;
    printf("Enter the product name to be searched: ");
    scanf(" %[^\n]", name);
    for(int i = 0; i < currentProductCapacity; i++)
    {
        if(strstr(product[i].name, name) != NULL)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", product[i].id, product[i].name, product[i].price, product[i].quantity);
            found = true;
        }
    }
    if(!found)
    {
        printf("Product with name %s not found.\n", name);
    }
}


void searchProductByPrice(Product *product, int currentProductCapacity)
{
    float minPrice;
    float maxPrice;
    bool found = false;

    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    for(int i = 0; i < currentProductCapacity; i++)
    {
        if (product[i].price >= minPrice && product[i].price <= maxPrice)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", product[i].id, product[i].name, product[i].price, product[i].quantity);
            found = true;
        }
    }
    if(!found)
    {
        printf("No products found in this price range.\n");
    }

}


void deleteProduct(Product **product, int *currentProductCapacity)
{
    int id;
    int index = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    for(int i = 0; i < *currentProductCapacity; i++)
    {
        if((*product)[i].id == id)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("Product not found.\n");
        return;
    }

    for(int i = index; i < *currentProductCapacity - 1; i++)
    {
        (*product)[i] = (*product)[i + 1];
    }

    (*currentProductCapacity)--;

    printf("Product with ID %d deleted successfully.\n", id);

    *product = (Product *)realloc(*product, (*currentProductCapacity) * sizeof(Product));

}
