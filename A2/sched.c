#include "sched.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_PROCESSES 100
#define LINE_SIZE 256

char *PolicyCheck(char *argOne);

char *RoundRobinIntegerCheck(char *argTwo);

int FileCheck(char *argthree, char *argtwo, char *scheduleType, char *fileName);

int FileReader(char *fileName, char *path, Process processes[], int *count);

void runFCFS(Process p[], int n);

void runRR(Process p[], int n);

int N;

int main(int argc, char *argv[]) 
{
    char *scheduleType;
    char *quantumCheck;
    char fileName[256];
    char path[500];

    //the checks below are to check if the first argument is asking to use Round Robin or First come first serve
    scheduleType = PolicyCheck(argv[1]);
    if (scheduleType == NULL) 
    {
        return 1;
    }

    //this will check the second argument if the schedule type is Round Robin, it will ensure the integer is greater than 0.
    if (strcmp(scheduleType, "Round Robin") == 0)
    {
        quantumCheck = RoundRobinIntegerCheck(argv[2]);
        if (quantumCheck == NULL) 
        {
            return 1;
        }
    }

    //this code will check the schedule type and get the file name from the user based on the scheduler they are using
    if(FileCheck(argv[3], argv[2], scheduleType, fileName) == 1)
    {
        return 1;
    }

    //this code will open the file, print the contents of the file, and close the file
    Process processes[MAX_PROCESSES];
    int processCount = 0;

    if(FileReader(fileName, path, processes, &processCount) == 1)
    {
        return 1;
    }

    printf("Read %d processes\n", processCount);

    //check if the process is FCFS, if it is go to the function runFCFS
    if(strcmp(scheduleType, "First come first serve")==0)
    {
        runFCFS(processes, processCount);
    }
    else if(strcmp(scheduleType, "Round Robin")==0)
    {
        runRR(processes, processCount);
    }

    return 0;
}



/*void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

int isEmpty(Queue *q)
{
    return q->front == NULL;
}

void enqueue(Queue* q, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

int dequeue(Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    Node* temp = q->front;
    int value = temp->data;

    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
    return value;
}*/




char *PolicyCheck(char *argOne)
{
    //the checks below are to check if the first argument is asking to use Round Robin or First come first serve
    if(strcmp(argOne, "--policy=RR") == 0)
    {
        return "Round Robin";
    }
    else if(strcmp(argOne, "--policy=FCFS") == 0)
    {
        return "First come first serve";
    }
    else
    {
        fprintf(stderr, "Error: invalid argument, use either --policy=RR OR --policy-FCFS\n");
        return NULL;
    }
}

char *RoundRobinIntegerCheck(char *argTwo)
{
    //this will check the second argument if the schedule type is Round Robin, it will ensure the integer is greater than 0.
        if (sscanf(argTwo, "--quantum=%d", &N) == 1)
        {
            if (N > 0)
            {
                return "True";
            }
            else
            {
                fprintf(stderr, "Error: Quantum must be greater than 0.\n");
                return NULL;
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid format for quantum. Use --quantum=N\n");
            return NULL;
        }
    
}

int FileCheck(char *argthree, char *argtwo, char *scheduleType, char *fileName)
{
    //this code will check the schedule type and get the file name from the user based on the scheduler they are using
    if(strcmp(scheduleType, "First come first serve")==0)
    {
        if (sscanf(argtwo, "--in=%s", fileName) == 1)
        {
            return 0;
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }
    else if(strcmp(scheduleType, "Round Robin")==0)
    {
        if (sscanf(argthree, "--in=%s", fileName) == 1)
        {
            return 0;
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

int FileReader(char *fileName, char *path, Process processes[], int *count)
{
    //this code will open the file, print the contents of the file, and close the file
    sprintf(path, "/workspaces/CMPT_360_Assignment_2/A2/Workload_Samples/%s", fileName);
    FILE *fp = fopen(path, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 1;
    }
    char *line = malloc(LINE_SIZE);
    if (!line) 
    {
        perror("malloc");
        return;
    }
    *count = 0;

    while (fgets(line, LINE_SIZE, fp)) {

        // skip comments
        if (line[0] == '#')
            continue;

        // parse: PID, ARRIVAL, CPU_TIME
        if (sscanf(line, "%d %d %d",
                   &processes[*count].pid,
                   &processes[*count].arrival,
                   &processes[*count].burst))
        {
            (*count)++;
        }
    }

    fclose(fp);

    free(line);

    return 0;
}

void runFCFS(Process p[], int n)
{
    //local variables to track time, context switches and TAT and RESP time
    int time = 0;
    int ctx_switches = 0;
    double totalTAT = 0, totalRESP = 0;
    int timeTracker = 0;

    //this will print the total time it will complete to run each process
    printf("time: ");
    int totalBurstArrival = 0;
    int totalBurst = 0;
    for(int i = 0; i < n; i++)
    {
        totalBurst += p[i].burst;
        totalBurstArrival += p[i].arrival;
        if(totalBurstArrival > totalBurst)
        {
            totalBurst = totalBurstArrival;
        }
    }
    for(int t = 0; t < totalBurst; t++)
    {
        printf("%d ",t);
    }
    printf("\n");

    //this will run the simulation
    printf("run: ");
    for(int i = 0; i < n; i++)
    {
        //this will create idle time
        while(timeTracker < p[i].arrival)
        {
            printf("- ");
            timeTracker++;
        }

        if(time < p[i].arrival)
        {
            time = p[i].arrival;
        }
        p[i].firstRun = time;
        p[i].RESP = p[i].firstRun - p[i].arrival;
        
        if(i>0)
        {
            ctx_switches++;
        }

        printf(" ");
        for (int t = 0; t < p[i].burst; t++)
        {
            printf("%d ", p[i].pid);
            time++;
            timeTracker++;
        }

        p[i].completion = time;
        p[i].TAT = p[i].completion - p[i].arrival;

        totalTAT += p[i].TAT;
        totalRESP += p[i].RESP;
    }

    printf("\n");
    
    //per process outputs
    for (int i = 0; i < n; i++)
    {
        printf("P%d: firstrun=%d completion=%d TAT=%d RESP=%d\n",p[i].pid,p[i].firstRun,p[i].completion,p[i].TAT,p[i].RESP);
    }

    //system stats
    printf("System: ctx_switches=%d, avgTAT=%.3f, avgRESP=%.3f\n",ctx_switches,totalTAT / n,totalRESP / n);

}

void runRR(Process p[], int n)
{
    Queue q;
    initQueue(&q);

    bool arrived_or_queued[MAX_PROCESSES] = {false};

    int time = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        p[i].remainingTime = p[i].burst;
        p[i].firstRun      = -1;
        p[i].completion    = 0;
        p[i].TAT           = 0;
        p[i].RESP          = 0;
    }

    #define MAX_TIMELINE 10000
    int run_at_time[MAX_TIMELINE];
    int timeline_len = 0;

    int current_idx = -1;
    int quantum_remaining = 0;

    while (completed < n) {

        //Add any newly arrived processes 
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time &&
                p[i].remainingTime > 0 &&
                !arrived_or_queued[i]) {
                enqueue(&q, i);
                arrived_or_queued[i] = true;
            }
        }

        //If CPU is free, pick next process 
        if (current_idx == -1) {
            if (!isEmpty(&q)) {
                current_idx = dequeue(&q);
                quantum_remaining = N;

                if (p[current_idx].firstRun == -1) {
                    p[current_idx].firstRun = time;
                    p[current_idx].RESP = time - p[current_idx].arrival;
                }
            }
        }

        
        if (current_idx != -1) {
            run_at_time[timeline_len] = p[current_idx].pid;
            p[current_idx].remainingTime--;
            quantum_remaining--;
            timeline_len++;
            time++;

            if (p[current_idx].remainingTime == 0) {
                p[current_idx].completion = time;
                p[current_idx].TAT = time - p[current_idx].arrival;
                completed++;
                current_idx = -1;
            }
            else if (quantum_remaining == 0) {
            
                enqueue(&q, current_idx);
                arrived_or_queued[current_idx] = true;
                current_idx = -1;
            }
        }
        else {
    
            run_at_time[timeline_len] = -1;
            timeline_len++;
            time++;
        }
    }

    //Calculate ctx_switches
    int ctx_switches = 0;
    int prev = -1;
    for (int t = 0; t < timeline_len; t++) {
        int cur = run_at_time[t];
        if (cur >= 0 && prev >= 0 && cur != prev) {
            ctx_switches++;
        }
        if (cur >= 0) {
            prev = cur;
        }
    }

    printf("time: ");
    for (int t = 0; t < timeline_len; t++)
        printf("%d ", t);
    printf("\nrun : ");
    for (int t = 0; t < timeline_len; t++) {
        if (run_at_time[t] == -1)
            printf("- ");
        else
            printf("%d ", run_at_time[t]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("P%d: first run=%d completion=%d TAT=%d RESP=%d\n",
               p[i].pid, p[i].firstRun, p[i].completion, p[i].TAT, p[i].RESP);
    }

    double avg_tat = 0.0, avg_resp = 0.0;
    for (int i = 0; i < n; i++) {
        avg_tat += p[i].TAT;
        avg_resp += p[i].RESP;
    }
    printf("System: ctx_switches=%d, avgTAT=%.3f, avgRESP=%.3f\n",
           ctx_switches, avg_tat / n, avg_resp / n);
}