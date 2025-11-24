#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TABLE_SIZE 1000

typedef struct Queue
{
    int key;
    char value[100];
    struct Queue *prev;
    struct Queue *next;

} Queue;

typedef struct LRU
{
    int capacity;
    int size;
    Queue *head;
    Queue *tail;
} LRU;

LRU *cache;

typedef struct HashMap
{
    int key;
    Queue *node;
    struct HashMap *next;
} HashMap;

HashMap *hashTable[TABLE_SIZE];

void createCache(int size);
int hash(int key);
void putKeyValue(int key, char *value);
void insertAtHead(Queue *node);
void mapPut(int key, Queue *node);
Queue *mapGet(int key);
void moveToHead(Queue *node);
Queue *removeTail();
void mapRemove(int key);
char *getValue(int key);

int main()
{
    char commandLine[100];
    char command[50];
    bool turn = true;
    
    while(turn)
    {
        fgets(commandLine, sizeof(commandLine), stdin);
        sscanf(commandLine, "%s", command);

        if(strcmp(command, "createCache") == 0)
        {
            int size;
            sscanf(commandLine, "%*s %d", &size);
            createCache(size);
        }
        else if(strcmp(command, "put") == 0)
        {
            int key;
            sscanf(commandLine, "%*s %d", &key);
            char value[100];
            sscanf(commandLine, "%*s %*d %s", value);
            putKeyValue(key, value);
        }
        else if(strcmp(command, "get") == 0)
        {
            int key;
            sscanf(commandLine, "%*s %d", &key);
            char *value = getValue(key);
            if(value != NULL)
            {
                printf("%s\n", value);
            }
        }
        else if(strcmp(command, "exit") == 0)
        {
            turn = false;
        }
    }
    return 0;
}

void createCache(int size)
{
    cache = (LRU *)malloc(sizeof(LRU));
    cache->capacity = size;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for(int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable[i] = NULL;
    }
}

int hash(int key)
{
    return key % TABLE_SIZE;
}

void putKeyValue(int key, char *value)
{
    Queue *node = mapGet(key);

    if(node != NULL)
    {
        strcpy(node->value, value);
        moveToHead(node);
        return;
    }

    Queue *newNode = (Queue *)malloc(sizeof(Queue));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = NULL;
    newNode->next = NULL;

    insertAtHead(newNode);
    mapPut(key, newNode);

    cache->size++;

    if(cache->size > cache->capacity)
    {
        Queue *temp = removeTail();
        mapRemove(temp->key);
        free(temp);
        cache->size--;
    }
}

void insertAtHead(Queue *node)
{
    node->next = cache->head;

    if(cache->head != NULL)
    {
        cache->head->prev = node;
    }

    cache->head = node;

    if(cache->tail == NULL)
    {
        cache->tail = node;
    }
}

void mapPut(int key, Queue *node)
{
    int hashValue = hash(key);
    HashMap *entry = (HashMap *)malloc(sizeof(HashMap));

    entry->key = key;
    entry->node = node;
    entry->next = hashTable[hashValue];
    hashTable[hashValue] = entry;
}

Queue *mapGet(int key)
{
    int hashValue = hash(key);
    HashMap *temp = hashTable[hashValue];

    while(temp != NULL)
    {
        if(temp->key == key)
        {
            return temp->node;
        }
        temp = temp->next;
    }
    return NULL;
}

void mapRemove(int key)
{
    int hashValue = hash(key);
    HashMap *temp = hashTable[hashValue];
    HashMap *prev = NULL;

    while(temp != NULL)
    {
        if(temp->key == key)
        {
            if(prev != NULL)
            {
                prev->next = temp->next;
            }
            else
            {
                hashTable[hashValue] = temp->next;
            }
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void moveToHead(Queue *node)
{
    if(node == cache->head)
    {
        return;
    }

    if(node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    if(node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    if(node == cache->tail)
    {
        cache->tail = node->prev;
    }

    node->prev = NULL;
    node->next = cache->head;

    if(cache->head != NULL)
    {
        cache->head->prev = node;
    }

    cache->head = node;

    if(cache->tail == NULL)
    {
        cache->tail = node;
    }
}

Queue *removeTail()
{
    Queue *temp = cache->tail;

    if(temp->prev != NULL)
    {
        cache->tail = temp->prev;
        cache->tail->next = NULL;
    }
    else
    {
        cache->head = NULL;
        cache->tail = NULL;
    }

    return temp;
}

char *getValue(int key)
{
    Queue *node = mapGet(key);

    if(node == NULL)
    {
        printf("NULL\n");
        return NULL;
    }

    moveToHead(node);
    return node->value;
}