#include "sched.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_PROCESSES 100
#define LINE_SIZE 256

char *PolicyCheck(char *argOne);

char *RoundRobinIntegerCheck(char *argTwo);

int FileCheck(char *argthree, char *argtwo, char *scheduleType, char *fileName);

int FileReader(char *fileName, char *path, Process processes[], int *count);

void runFCFS(Process p[], int n);

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
    runFCFS(processes, processCount);

    return 0;
}

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