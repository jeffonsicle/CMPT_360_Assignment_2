#ifndef SCHED_H
#define SCHED_H

typedef struct {
    int pid;
    int arrival;
    int burst;
    int firstRun;
    int completion;
    int TAT;
    int RESP;
} Process;

int FileReader(char *fileName, char *path, Process processes[], int *count);
void runFCFS(Process p[], int n);

#endif
