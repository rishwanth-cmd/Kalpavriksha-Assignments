#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Players_data.h"

#define MAX_NAME_LENGTH 50
#define MAX_TEAMS 10

typedef enum
{
    BATSMAN = 1,
    BOWLER = 2,
    ALLROUNDER = 3
} PlayerRole;

const char* roleToString(PlayerRole role)
{
    switch(role)
    {
        case BATSMAN: 
            return "Batsman";
        case BOWLER: 
            return "Bowler";
        case ALLROUNDER: 
            return "All-rounder";
        default: 
            return "Unknown";
    }
}

typedef struct Player
{
    int playerId;
    char name[50];
    char teamName[50];
    PlayerRole role;   
    int totalRuns;
    float battingAvg;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct Player *next;
} Player;

typedef struct Team
{
    int teamId;
    char name[MAX_NAME_LENGTH];
    int totalPlayers;
    float avgBatStrikeRate;
    float batSRsum;
    int batsmenCount;
    Player *batsmenHead;
    Player *bowlersHead;
    Player *allroundersHead;
} Team;

typedef struct TempNode
{
    Player *player;
    struct TempNode *next;
} TempNode;

Team Teams[MAX_TEAMS];

void addFromDataSet();
float computePerformanceIndex(Player *player);
void appendPlayer(Team *teams, Player *player);
void insertSorted(Player **head, Player *player);
void printPlayerHeader();
void printPlayer(Player *player);
void displayTeamPlayers();
int findTeamIndex(int teamId);
void addPlayer();
void displayTeamsByAvgSR();
void displayTopKPlayers();
void displayAllPlayersByPerformanceIndex();
void insertSortedTemp(TempNode **head, Player *player);

int main()
{
    addFromDataSet();
    bool turn = true;

    while (turn)
    {
        int choice;
        printf("ICC ODI Player Performance Analyzer\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Avg Batting SR\n");
        printf("4. Display Top K Players of Team by Role\n");
        printf("5. Display All Players of a Role Across Teams\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: 
                addPlayer();
                break;
            case 2:
                displayTeamPlayers();
                break;
            case 3:
                displayTeamsByAvgSR();
                break;
            case 4:
                displayTopKPlayers();
                break;
            case 5: 
                displayAllPlayersByPerformanceIndex();
                break;
            case 6: 
                printf("Freed Memory.\n");
                turn = false;
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void addFromDataSet()
{
    for(int i = 0; i < teamCount; i++)
    {
        Teams[i].teamId = i + 1;
        strcpy(Teams[i].name, teams[i]);
        Teams[i].totalPlayers = 0;
        Teams[i].batSRsum = 0;
        Teams[i].batsmenCount = 0;
        Teams[i].avgBatStrikeRate = 0;
        Teams[i].batsmenHead = NULL;
        Teams[i].bowlersHead = NULL;
        Teams[i].allroundersHead = NULL;
    }

    for(int i = 0; i < playerCount; i++)
    {
        Player *player = (Player *)malloc(sizeof(Player));

        player->playerId = players[i].id;
        strcpy(player->name, players[i].name);
        strcpy(player->teamName, players[i].team);
        if(strcmp(players[i].role, "Batsman") == 0) 
        {
            player->role = BATSMAN;
        }
        else if(strcmp(players[i].role, "Bowler") == 0) 
        {
            player->role = BOWLER;
        }
        else
        {
            player->role = ALLROUNDER;
        }
        player->totalRuns = players[i].totalRuns;
        player->battingAvg = players[i].battingAverage;
        player->strikeRate = players[i].strikeRate;
        player->wickets = players[i].wickets;
        player->economyRate = players[i].economyRate;
        player->performanceIndex = computePerformanceIndex(player);
        player->next = NULL;

        for (int j = 0; j < teamCount; j++)
        {
            if (strcmp(Teams[j].name, player->teamName) == 0)
            {
                appendPlayer(&Teams[j], player);
                break;
            }
        }
    }
}

float computePerformanceIndex(Player *player)
{
    if(player->role == BATSMAN)
    {
        return (player->battingAvg * player->strikeRate) / 100.0f;
    }
    if(player->role == BOWLER)
    {
        return (player->wickets * 2) + (100 - player->economyRate);
    }
    return (player->battingAvg * player->strikeRate) / 100.0f + (player->wickets * 2);
}

void appendPlayer(Team *team, Player *player)
{
    team->totalPlayers++;

    if(player->role == BATSMAN || player->role == ALLROUNDER)
    {
        team->batSRsum += player->strikeRate;
        team->batsmenCount++;
        team->avgBatStrikeRate = team->batSRsum / team->batsmenCount;
    }

    if(player->role == BATSMAN)
    {
        insertSorted(&team->batsmenHead, player);
    }
    else if(player->role == BOWLER)
    {
        insertSorted(&team->bowlersHead, player);
    }
    else
    {
        insertSorted(&team->allroundersHead, player);
    }
}


void insertSorted(Player **head, Player *player)
{
    if(*head == NULL || (*head)->performanceIndex < player->performanceIndex)
    {
        player->next = *head;
        *head = player;
        return;
    }

    Player *currentPlayer = *head;
    while(currentPlayer->next && currentPlayer->next->performanceIndex >= player->performanceIndex)
    {
        currentPlayer = currentPlayer->next;
    }

    player->next = currentPlayer->next;
    currentPlayer->next = player;
}


void printPlayerHeader()
{
    printf("====================================================================================\n");
    printf("ID   Name                      Role           Runs  Avg    SR     Wkts  ER   PerfIdx\n");
    printf("====================================================================================\n");
}

void printPlayer(Player *player)
{
    printf("%-4d %-24s %-14s %-6d %-6.1f %-6.1f %-5d %-5.1f %-7.2f\n",
        player->playerId,
        player->name,
        roleToString(player->role),
        player->totalRuns,
        player->battingAvg,
        player->strikeRate,
        player->wickets,
        player->economyRate,
        player->performanceIndex
    );
}

void displayTeamPlayers()
{
    int id;
    printf("Enter Team ID: ");
    scanf("%d", &id);

    int index = findTeamIndex(id);
    if(index == -1)
    {
        printf("Invalid Team ID.\n");
        return;
    }

    Team *team = &Teams[index];

    printPlayerHeader();

    Player *player = team->batsmenHead;
    while(player != NULL)
    { 
        printPlayer(player);
        player = player->next;
    }

    player = team->bowlersHead;

    while(player != NULL)
    {
        printPlayer(player);
        player = player->next;
    }

    player = team->allroundersHead;

    while(player != NULL)
    { 
        printPlayer(player);
        player = player->next;
    }

    printf("====================================================================================\n");
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->avgBatStrikeRate);
}

int findTeamIndex(int teamId)
{
    int low = 0;
    int high = teamCount - 1;

    while(low <= high)
    {
        int mid = (low + high) / 2;

        if(Teams[mid].teamId == teamId)
        {
            return mid;
        }
        if(Teams[mid].teamId < teamId)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

void addPlayer()
{
    int teamId;
    printf("Enter Team ID (1-%d): ", teamCount);
    scanf("%d", &teamId);

    int index = findTeamIndex(teamId);
    if(index == -1)
    { 
        printf("Invalid Team\n"); 
        return; 
    }

    Player *player = (Player *)malloc(sizeof(Player));

    printf("Player ID: ");
    scanf("%d", &player->playerId);
    getchar();

    printf("Name: ");
    fgets(player->name, MAX_NAME_LENGTH, stdin);
    player->name[strlen(player->name) - 1] = '\0';

    strcpy(player->teamName, Teams[index].name);

    int role;
    printf("Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
    scanf("%d", &role);

    player->role = (PlayerRole)role;

    printf("Total Runs: ");
    scanf("%d", &player->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &player->battingAvg);

    printf("Strike Rate: ");
    scanf("%f", &player->strikeRate);

    printf("Wickets: ");
    scanf("%d", &player->wickets);

    printf("Economy Rate: ");
    scanf("%f", &player->economyRate);

    player->performanceIndex = computePerformanceIndex(player);
    player->next = NULL;

    appendPlayer(&Teams[index], player);

    printf("Player Added Successfully!\n");
}

void displayTeamsByAvgSR()
{
    Team temp[MAX_TEAMS];
    for(int i = 0; i < teamCount; i++)
    {
        temp[i] = Teams[i];
    }

    for(int i = 0; i < teamCount - 1; i++)
    {
        for(int j = 0; j < teamCount - i - 1; j++)
        {
            if(temp[j].avgBatStrikeRate < temp[j+1].avgBatStrikeRate)
            {
                Team t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
    }

    printf("ID   Team Name           Avg Bat SR   Total Players\n");
    printf("===================================================\n");

    for(int i = 0; i < teamCount; i++)
    {
        printf("%-4d %-18s %-15.2f %-5d\n", temp[i].teamId, temp[i].name, temp[i].avgBatStrikeRate, temp[i].totalPlayers);
    }

    printf("===================================================\n");
}

void displayTopKPlayers()
{
    int teamId;
    int k;
    int role;

    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    int index = findTeamIndex(teamId);
    if(index == -1)
    { 
        printf("Invalid Team ID\n"); 
        return;
    }

    printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
    scanf("%d", &role);

    printf("Enter K: ");
    scanf("%d", &k);

    Player *head = NULL;

    if(role == BATSMAN)
    {
        head = Teams[index].batsmenHead;
    }
    else if(role == BOWLER)
    {
        head = Teams[index].bowlersHead;
    }
    else
    {
        head = Teams[index].allroundersHead;
    }

    if(head == NULL)
    {
        printf("No players for this role\n");
        return;
    }

    printPlayerHeader();

    int count = 0;
    Player *player = head;

    while(player && count < k)
    {
        printPlayer(player);
        player = player->next;
        count++;
    }

    printf("====================================================================================\n");
}

void displayAllPlayersByPerformanceIndex()
{
    int role;
    char roleText[20];

    printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
    scanf("%d", &role);

    if(role == BATSMAN)
    {
        strcpy(roleText, "Batsman");
    }
    else if(role == BOWLER)
    {
        strcpy(roleText, "Bowler");
    }
    else
    {
        strcpy(roleText, "All-rounder");
    }

    TempNode *head = NULL;

    for(int i = 0; i < teamCount; i++)
    {
        Player *player = NULL;

        if(role == BATSMAN)
        {
            player = Teams[i].batsmenHead;
        }
        else if(role == BOWLER)
        {
            player = Teams[i].bowlersHead;
        }
        else
        {
            player = Teams[i].allroundersHead;
        }

        while(player != NULL)
        {
            insertSortedTemp(&head, player);
            player = player->next;
        }
    }

    if(head == NULL)
    {
        printf("No players found for role %s\n", roleText);
        return;
    }

    printf("Players Across All Teams (Role = %s)\n", roleText);
    printf("==============================================================================================\n");
    printf("ID   Name                 Team              Role           Runs  Avg   SR    Wkts  ER   PerfIdx\n");
    printf("==============================================================================================\n");

    TempNode *currentPlayer = head;

    while(currentPlayer != NULL)
    {
        Player *player = currentPlayer->player;

        printf("%-4d %-20s %-16s %-15s %-5d %-5.1f %-5.1f %-4d %-4.1f %-7.2f\n",
            player->playerId,
            player->name,
            player->teamName,
            roleToString(player->role),
            player->totalRuns,
            player->battingAvg,
            player->strikeRate,
            player->wickets,
            player->economyRate, 
            player->performanceIndex);

        currentPlayer = currentPlayer->next;
    }

    while(head != NULL)
    {
        TempNode *temp = head;
        head = head->next;
        free(temp);
    }
}

void insertSortedTemp(TempNode **head, Player *player)
{
    TempNode *newNode = (TempNode *)malloc(sizeof(TempNode));
    newNode->player = player;
    newNode->next = NULL;

    if(*head == NULL || (*head)->player->performanceIndex < player->performanceIndex)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    TempNode *currentPlayer = *head;
    while(currentPlayer->next && currentPlayer->next->player->performanceIndex >= player->performanceIndex)
    {
        currentPlayer = currentPlayer->next;
    }

    newNode->next = currentPlayer->next;
    currentPlayer->next = newNode;
}