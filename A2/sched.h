#ifndef SCHED_H
#define SCHED_H
#define MAX_PROCESSES 100
#include <stdio.h> 
#include <stdbool.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int firstRun;
    int completion;
    int TAT;
    int RESP;
    int remainingTime;
} Process;

typedef struct {
    int data[MAX_PROCESSES];
    int front;
    int rear;
    int size;         
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear  = 0;
    q->size  = 0;
}

bool isEmpty(Queue *q) {
    return q->size == 0;}

bool isFull(Queue *q) {
    return q->size == MAX_PROCESSES;
}

void enqueue(Queue *q, int x) {
    if (isFull(q)) {
        fprintf(stderr, "Queue overflow!\n");
        exit(1);          
    }
    q->data[q->rear] = x;
    q->rear = (q->rear + 1) % MAX_PROCESSES;
    q->size++;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue underflow!\n");
        exit(1);
    }
    int val = q->data[q->front];
    q->front = (q->front + 1) % MAX_PROCESSES;
    q->size--;
    return val;
}

int FileReader(char *fileName, char *path, Process processes[], int *count);
void runFCFS(Process p[], int n);
void runRR(Process p[], int n);

#endif
