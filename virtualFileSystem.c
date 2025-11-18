#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_LINE 2048
#define MAX_NAME 50

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;


FreeBlock *head = NULL;
FreeBlock *tail = NULL;

typedef struct FileNode
{
    char name[MAX_NAME];
    int isDirectory;                
    struct FileNode *next;
    struct FileNode *prev;   
    struct FileNode *parent;        
    struct FileNode *child;
    int *blockPointers;             
    int blockCount;                 
    int fileSize;                  
} FileNode;

FileNode *root = NULL;  
FileNode *cwd = NULL;   

void initializeFreeBlocks();
FileNode* createNode(const char *name, int isDirectory, FileNode *parent);
void mkdirCommand(char *name);
void lsCommand();
void cdCommand(char *name);
void pwdCommand();
void rmdirCommand(char *name);
void createCommand(char *name);
void writeCommand(char *fileName, char *content);
void readCommand(char *fileName);
void deleteCommand(char *fileName);
void dfCommand();
void freeFileNode(FileNode *node);
void freeFreeBlock();

static unsigned char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];


int main()
{
    initializeFreeBlocks();
    root = createNode("/", 1, NULL);
    root->next = root->prev = root;
    cwd = root;
    printf("Ready.\n");

    char line[MAX_LINE];
    char cmd[50];
    char arg1[100];
    char arg2[1000];

    bool turn = true;
    while(turn)
    {
        if(cwd == root)
        {
            printf("/ >");
        }
        else
        {
            printf("%s > ", cwd->name);
        }

        fgets(line, sizeof(line), stdin);
        line[strlen(line)-1]='\0'; 

        if(sscanf(line, "mkdir %s", arg1) == 1)
        {
            mkdirCommand(arg1);
        } 
        else if(strcmp(line, "ls") == 0)
        {
            lsCommand();
        }
        else if(sscanf(line, "cd %s", arg1) == 1)
        {
            cdCommand(arg1);
        }
        else if(strcmp(line, "pwd") == 0)
        {
            pwdCommand();
        }
        else if(sscanf(line, "rmdir %s", arg1) == 1)
        {
            rmdirCommand(arg1);
        }
        else if(sscanf(line, "create %s", arg1) == 1)
        {
            createCommand(arg1);
        }
        else if(strncmp(line, "write ", 6) == 0)
        {
            sscanf(line, "write %s %[^\n]", arg1, arg2);
            writeCommand(arg1, arg2);
        }
        else if(sscanf(line, "read %s", arg1) == 1)
        {
            readCommand(arg1);
        }
        else if(sscanf(line, "delete %s", arg1) == 1)
        {
            deleteCommand(arg1);
        }
        else if(strcmp(line, "df") == 0)
        {
            dfCommand();
        }
        else if (strcmp(line, "exit") == 0)
        {
            freeFileNode(root);
            freeFreeBlock();
            printf("Exit\n");
            turn = false;
        }
        else
        {
            printf("Unknown command.\n");
        }
    }
}

void initializeFreeBlocks()
{
    for(int i = 0; i < NUM_BLOCKS; i++)
    {
        FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
        node->index = i;
        node->next = node->prev = NULL;

        if(!head)
        {
            head = tail = node;
        }
        else
        {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }
}

int popFreeBlock()
{
    if(!head)
    {
        return -1;
    }
    int index = head->index;
    FreeBlock *temp = head;
    head = head->next;
    if(head)
    {
        head->prev = NULL;
    }
    else 
    {
        tail = NULL;
    }
    free(temp);
    return index;
}

void pushFreeBlock(int index)
{
    FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
    node->index = index;
    node->next = NULL;
    node->prev = tail;

    if(!head)
    {
        head = tail = node;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
}

int countFreeBlocks()
{
    int count = 0;
    for(FreeBlock *temp = head; temp != NULL; temp = temp->next)
    {
        count++;
    }
    return count;
}

FileNode* createNode(const char *name, int isDirectory, FileNode *parent)
{
    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    strcpy(node->name, name);
    node->isDirectory = isDirectory;
    node->next = node->prev = node->child = NULL;
    node->parent = parent;
    node->blockPointers = NULL;
    node->blockCount = 0;
    node->fileSize = 0;
    return node;
}


void insertChild(FileNode *parent, FileNode *child)
{
    if(!parent)
    {
        return;
    }
    if(!parent->child)
    {
        parent->child = child;
        child->next = child->prev = child;
    } 
    else 
    {
        FileNode *head = parent->child;
        FileNode *tail = head->prev;
        tail->next = child;
        child->prev = tail;
        child->next = head;
        head->prev = child;
    }
    child->parent = parent;
}

FileNode* findChild(FileNode *parent, const char *name)
{
    if(!parent || !parent->child)
    {
        return NULL;
    }

    FileNode *temp = parent->child;

    do {
        if(strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != parent->child);

    return NULL;
}

void mkdirCommand(char *name)
{
    if(!name)
    {
        printf("Please enter a valid directory name.\n");
        return;
    }
    if(strcmp(name, "..") == 0)
    {
        printf("Invalid directory name: '..' is reserved.\n");
        return;
    }
    if(findChild(cwd, name))
    {
        printf("Directory already exists!\n");
        return;
    }

    FileNode *directory = createNode(name, 1, cwd);
    insertChild(cwd, directory);
    printf("Directory '%s' created.\n", name);
}

void lsCommand()
{
    if(!cwd->child)
    { 
        printf("Empty.\n");
        return; 
    }
    FileNode *temp = cwd->child;
    do {
        if(temp->isDirectory == 1)
        {
            printf("%s/\n", temp->name);
        }
        else
        {
            printf("%s\n", temp->name);
        }
        temp = temp->next;
    } while (temp != cwd->child);
}

void cdCommand(char *name)
{
    if(!name)
    { 
        printf("Please enter a valid directory name.\n");
        return;
    }
    if(strcmp(name, "..") == 0 && cwd->parent)
    {
        cwd = cwd->parent;
        return;
    }
    FileNode *directory = findChild(cwd, name);
    if(directory && directory->isDirectory)
    {
        cwd = directory;
    }
    else
    {
        printf("Directory not found.\n");    
    }
}

void pwdCommand()
{
    if(cwd == root)
    {
        printf("/\n");
        return;
    }

    char path[1000] = "";
    FileNode *temp = cwd;
    char *parts[100];
    int count = 0;

    while(temp && temp != root)
    {
        parts[count++] = temp->name;
        temp = temp->parent;
    }

    for(int i = count - 1; i >= 0; i--)
    {
        strcat(path, "/");
        strcat(path, parts[i]);
    }

    printf("%s\n", path);
}

void rmdirCommand(char *name)
{
    if(!name)
    {
        printf("Please enter a valid directory name.\n");
        return;
    }
    FileNode *directory = findChild(cwd, name);

    if(!directory || !directory->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }

    if(directory->child != NULL)
    {
        printf("Directory '%s' is not empty.\n", name);
        return;
    }

    if(directory->next == directory)
    {
        cwd->child = NULL;
    }
    else
    {
        directory->prev->next = directory->next;
        directory->next->prev = directory->prev;
        if(cwd->child == directory)
        {
            cwd->child = directory->next;
        }
    }

    free(directory);
    printf("Directory '%s' removed.\n", name);
}

void createCommand(char *name)
{
    if(!name)
    { 
        printf("Please enter a valid file name.\n");
        return;
    }
    if(findChild(cwd, name))
    {
        printf("File already exists!\n");
        return;
    }

    FileNode *file = createNode(name, 0, cwd);
    insertChild(cwd, file);
    printf("File '%s' created.\n", name);
}

void writeCommand(char *fileName, char *content)
{
    FileNode *file = findChild(cwd, fileName);
    if(!file || file->isDirectory)
    { 
        printf("File not found.\n");
        return; 
    }

    if(file->blockPointers != NULL && file->blockCount > 0)
    {
        for(int i = 0; i < file->blockCount; i++)
        {
            pushFreeBlock(file->blockPointers[i]);  
        }
        free(file->blockPointers);
        file->blockPointers = NULL;
        file->blockCount = 0;
        file->fileSize = 0;
    }


    int bytes = strlen(content);
    int blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    if(countFreeBlocks() < blocks)
    { 
        printf("Disk full!\n");
        return; 
    }



    file->blockPointers = malloc(sizeof(int) * blocks);
    file->blockCount = blocks;
    file->fileSize = bytes;

    int remainingBytes = bytes;

    for(int i = 0; i < blocks; i++)
    {
        int index = popFreeBlock();
        file->blockPointers[i] = index;
        int toCopy = (remainingBytes > BLOCK_SIZE) ? BLOCK_SIZE : remainingBytes;
        memcpy(virtualDisk[index], content + i * BLOCK_SIZE, toCopy);
        remainingBytes -= toCopy;
    }
    printf("Wrote %d bytes to '%s'\n", file->fileSize, fileName);
}

void readCommand(char *fileName)
{
    FileNode *file = findChild(cwd, fileName);
    if(!file || file->isDirectory)
    {
        printf("File not found.\n"); 
        return;
    }

    int bytes = file->fileSize;
    for(int i = 0; i < file->blockCount && bytes > 0; i++)
    {
        int index = file->blockPointers[i];
        int toPrint = bytes > BLOCK_SIZE ? BLOCK_SIZE : bytes;
        fwrite(virtualDisk[index], 1, toPrint, stdout);
        bytes -= toPrint;
    }
    printf("\n");
}

void deleteCommand(char *fileName)
{
    FileNode *file = findChild(cwd, fileName);
    if(!file || file->isDirectory) 
    {
        printf("File not found.\n");
        return;
    }

    for(int i = 0; i < file->blockCount; i++)
    {
        pushFreeBlock(file->blockPointers[i]);
    }
    free(file->blockPointers);

    if(file->next == file)
    {
        cwd->child = NULL;
    }
    else
    {
        file->prev->next = file->next;
        file->next->prev = file->prev;
        if(cwd->child == file) 
        {
            cwd->child = file->next;
        }
    }
    free(file);
    printf("File deleted.\n");
}

void dfCommand()
{
    int freeCount = countFreeBlocks();
    int used = NUM_BLOCKS - freeCount;
    double usage = (NUM_BLOCKS == 0) ? 0.0 : (used * 100.0) / NUM_BLOCKS;

    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", used);
    printf("Free Blocks: %d\n", freeCount);
    printf("Disk Usage: %.2f%%\n", usage);
}

void freeFileNode(FileNode *node)
{
    if(!node)
    {
        return;
    }

    FileNode *child = node->child;

    if(child != NULL)
    {
        FileNode *first = child;
        child->prev->next = NULL; 
        child->prev = NULL;

        FileNode *temp = first;
        while(temp != NULL)
        {
            FileNode *next = temp->next;
            freeFileNode(temp);  
            temp = next;
        }
    }

    if(!node->isDirectory && node->blockPointers)
    {
        free(node->blockPointers);
    }

    free(node);
}

void freeFreeBlock()
{
    FreeBlock *temp = head;
    while(temp != NULL)
    {
        FreeBlock *next = temp->next;
        free(temp);
        temp = next;
    }
    head = tail = NULL;
}



