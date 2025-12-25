#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define HASH_SIZE 1009

typedef struct PCB
{
    char process_name[NAME_LEN];
    int process_id;
    int cpu_burst;
    int remaining_cpu;
    int io_start_time;
    int io_duration;
    int remaining_io;
    int io_started;
    int executed_cpu;
    int completion_time;
    int kill_time;
    int state;
    struct PCB *next;
} PCB;

typedef struct QueueNode
{
    PCB *process;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
    int size;
} Queue;

typedef struct KillEvent
{
    int target_pid;
    int kill_time;
    int used;
    struct KillEvent *next;
} KillEvent;

unsigned computeHash(int pid)
{
    return pid % HASH_SIZE;
}

void hashmapInsert(PCB *hash_table[], PCB *process)
{
    int h = computeHash(process->process_id);
    process->next = hash_table[h];
    hash_table[h] = process;
}

PCB *hashmapGet(PCB *hash_table[], int pid)
{
    int h = computeHash(pid);
    PCB *current = hash_table[h];

    while(current != NULL)
    {
        if(current->process_id == pid)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void hashmapRemove(PCB *hash_table[], int pid)
{
    int h = computeHash(pid);
    PCB *current = hash_table[h];
    PCB *prev = NULL;

    while(current != NULL)
    {
        if(current->process_id == pid)
        {
            if(prev != NULL)
            {
                prev->next = current->next;
            }
            else
            {
                hash_table[h] = current->next;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
}


void queueInit(Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void queuePush(Queue *q, PCB *p)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->process = p;
    node->next = NULL;

    if(!q->tail)
    {
        q->head = node;
        q->tail = node;
    }
    else
    {
        q->tail->next = node;
        q->tail = node;
    }

    q->size++;
}

PCB* queuePop(Queue *q)
{
    if(!q->head)
    {
        return NULL;
    }

    QueueNode *node = q->head;
    PCB *process = node->process;

    q->head = node->next;
    if(!q->head)
    {
        q->tail = NULL;
    }

    free(node);
    q->size--;

    return process;
}

int queueRemoveByPid(Queue *q, int pid)
{
    QueueNode *current = q->head;
    QueueNode *prev = NULL;

    while(current != NULL)
    {
        if(current->process->process_id == pid)
        {
            if(prev != NULL)
            {
                prev->next = current->next;
            }
            else
            {
                q->head = current->next;
            }

            if(current == q->tail)
            {
                q->tail = prev;
            }

            free(current);
            q->size--;
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}


void addKillEvent(KillEvent **kill_list, int pid, int time)
{
    KillEvent *k = (KillEvent *)malloc(sizeof(KillEvent));
    k->target_pid = pid;
    k->kill_time = time;
    k->used = 0;
    k->next = *kill_list;
    *kill_list = k;
}

int terminatedCount(Queue *terminated_queue)
{
    return terminated_queue->size;
}


void readInput(
    PCB *hash_table[],
    Queue *ready_queue,
    KillEvent **kill_list,
    int *total_processes
)
{
    char line[200];

    while(fgets(line, sizeof(line), stdin))
    {

        if(strncmp(line, "END", 3) == 0)
        {
            break;
        }

        if(strncmp(line, "kill", 4) == 0)
        {
            int pid;
            int time;
            if(sscanf(line + 4, "%d %d", &pid, &time) == 2)
            {
                addKillEvent(kill_list, pid, time);
            }
        }
        else
        {
            char pname[NAME_LEN];
            char io_start_str[20];
            char io_duration_str[20];
            
            int pid;
            int cpu;

            int count = sscanf(line, "%s %d %d %s %s", pname, &pid, &cpu, io_start_str, io_duration_str);

            if(count >= 3)
            {
                PCB *process = (PCB *)calloc(1, sizeof(PCB));

                strcpy(process->process_name, pname);
                process->process_id = pid;
                process->cpu_burst = cpu;
                process->remaining_cpu = cpu;

                if(count >= 5 && strcmp(io_start_str, "-") != 0)
                {
                    process->io_start_time = atoi(io_start_str);
                    process->io_duration = atoi(io_duration_str);
                }
                else
                {
                    process->io_start_time = -1;
                    process->io_duration = 0;
                }

                process->remaining_io = 0;
                process->io_started = 0;
                process->executed_cpu = 0;

                hashmapInsert(hash_table, process);
                queuePush(ready_queue, process);

                (*total_processes)++;
            }
        }
    }
}


void applyKillEvents(
    int tick,
    KillEvent *kill_list,
    PCB *hash_table[],
    Queue *ready_queue,
    Queue *waiting_queue,
    Queue *terminated_queue,
    PCB **current_running
)
{
    KillEvent *temp = kill_list;

    while(temp != NULL)
    {
        if(!temp->used &&temp->kill_time == tick)
        {
            PCB *process = hashmapGet(hash_table, temp->target_pid);

            if(process != NULL)
            {
                if(*current_running && (*current_running)->process_id == process->process_id)
                {
                    (*current_running)->state = 2;
                    (*current_running)->kill_time = tick;

                    hashmapRemove(hash_table, process->process_id);
                    queuePush(terminated_queue, *current_running);

                    *current_running = NULL;
                }
                else if(queueRemoveByPid(ready_queue, process->process_id))
                {
                    process->state = 2;
                    process->kill_time = tick;
                    hashmapRemove(hash_table, process->process_id);
                    queuePush(terminated_queue, process);
                }
                else if(queueRemoveByPid(waiting_queue, process->process_id))
                {
                    process->state = 2;
                    process->kill_time = tick;
                    hashmapRemove(hash_table, process->process_id);
                    queuePush(terminated_queue, process);
                }
            }

        temp->used = 1;
        }

        temp = temp->next;
    }
}


void simulate(
    PCB *hash_table[],
    Queue *ready_queue,
    Queue *waiting_queue,
    Queue *terminated_queue,
    KillEvent *kill_list,
    int total_processes
)
{
    int tick = 0;
    PCB *current_running = NULL;

    while(terminatedCount(terminated_queue) < total_processes)
    {
        applyKillEvents(
            tick,
            kill_list,
            hash_table,
            ready_queue,
            waiting_queue,
            terminated_queue,
            &current_running
        );

        Queue updated_waiting;
        queueInit(&updated_waiting);

        QueueNode *current = waiting_queue->head;

        while(current != NULL)
        {
            PCB *process = current->process;
            QueueNode *next = current->next;

            if(!process->io_started)
            {
                process->io_started = 1;
                queuePush(&updated_waiting, process);
            }
            else
            {
                if(process->remaining_io > 0)
                {
                    process->remaining_io--;
                }

                if(process->remaining_io == 0)
                {
                    process->state = 0;
                    process->io_started = 0;
                    queuePush(ready_queue, process);
                }
                else
                {
                    queuePush(&updated_waiting, process);
                }
            }

            free(current);
            current = next;
        }

        *waiting_queue = updated_waiting;

        if(!current_running)
        {
            current_running = queuePop(ready_queue);
            if(current_running != NULL)
            {
                current_running->state = 1;
            }
        }

        if(current_running != NULL)
        {
            current_running->executed_cpu++;
            current_running->remaining_cpu--;
        }

        if(current_running != NULL)
        {
            if(current_running->remaining_cpu == 0)
            {
                current_running->state = 3;
                current_running->completion_time = tick + 1;

                hashmapRemove(hash_table, current_running->process_id);
                queuePush(terminated_queue, current_running);

                current_running = NULL;
            }
            else if(current_running->io_start_time >= 0 && current_running->executed_cpu == current_running->io_start_time)
            {
                current_running->state = 4;
                current_running->remaining_io = current_running->io_duration;
                current_running->io_started = 0;

                queuePush(waiting_queue, current_running);
                current_running = NULL;
            }
        }

        tick++;
    }
}


void printResults(Queue *terminated_queue)
{
    printf("PID\tName\tCPU\tIO\tStatus\t\tTurnaround\tWaiting\n");

    QueueNode *current = terminated_queue->head;

    while(current != NULL)
    {
        PCB *process = current->process;

        if(process->state == 2)
        {
            printf("%d\t%s\t%d\t%d\tKILLED at %d\t-\t\t-\n", process->process_id, process->process_name, process->cpu_burst, process->io_duration, process->kill_time);
        }
        else
        {
            int turnaround = process->completion_time;
            int waiting = turnaround - process->cpu_burst;

            if(waiting < 0) waiting = 0;

            printf("%d\t%s\t%d\t%d\tOK\t\t%d\t\t%d\n", process->process_id, process->process_name, process->cpu_burst, process->io_duration, turnaround, waiting);
        }

        current = current->next;
    }
}


int main()
{
    PCB *hash_table[HASH_SIZE];
    for(int i=0;i<HASH_SIZE;i++)
    {
        hash_table[i] = NULL;
    }

    Queue ready_queue;
    Queue waiting_queue;
    Queue terminated_queue;

    queueInit(&ready_queue);
    queueInit(&waiting_queue);
    queueInit(&terminated_queue);

    KillEvent *kill_list = NULL;
    int total_processes = 0;

    readInput(hash_table, &ready_queue, &kill_list, &total_processes);

    if(total_processes == 0)
    {
        printf("No processes provided.\n");
        return 0;
    }

    simulate(
        hash_table,
        &ready_queue,
        &waiting_queue,
        &terminated_queue,
        kill_list,
        total_processes
    );

    printResults(&terminated_queue);

    return 0;
}
