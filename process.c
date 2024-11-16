#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "process.h"

bool hasPriority(IOType type){
  switch (type) {
    case TAPE: return true;
    case DISK: return false;
    case PRINTER: return true;
    default:
      printf("Invalid IOType passed to hasPriority\n");
      exit(1);
  }
}

Process* createEmptyProcess(int PID, char* name)  {
  Process* newProcess = malloc(sizeof(Process));
  if (newProcess == NULL) return NULL;

  newProcess->PID = PID;
  newProcess-> processName = malloc(strlen(name) * sizeof(char));
  strcpy(newProcess->processName, name);
  newProcess->firstNode = NULL; 
  return newProcess;
}

int addProcessNode(Process *process, int CPUTime, IOType IORequest) {
  ProcessNode* newNode = malloc(sizeof(ProcessNode));  
  if (newNode == NULL) return 0;

  newNode->CPUTime = CPUTime;
  newNode->IORequest = IORequest;
  newNode->nextNode = NULL;

  if (process->firstNode == NULL) {
    process->firstNode = newNode;
    return 1;
  }

  ProcessNode* currentNode = NULL;
  ProcessNode* nextNode = process->firstNode;
  while (nextNode != NULL) {
    currentNode = nextNode;
    nextNode = nextNode->nextNode;
  }
  currentNode->nextNode = newNode;
  return 2;
}

ProcessQueue* createProcessQueue() {
  ProcessQueue* newQueue = malloc(sizeof(ProcessQueue));
  if (newQueue == NULL) return NULL;

  newQueue->head = NULL;
  return newQueue;
}

int enQueueProcess(Process* process, ProcessQueue* queue) {
  if (queue == NULL) return 0;

  QueueNode* newNode = malloc(sizeof(QueueNode));
  if (newNode == NULL) return 0;

  newNode->process = process;
  newNode->nextQueueNode = NULL;

  if (queue->head == NULL) {
    queue->head = newNode;
    return 1;
  }

  QueueNode* currentNode = NULL;
  QueueNode* nextNode = queue->head;
  while (nextNode != NULL) {
    currentNode = nextNode;
    nextNode = nextNode->nextQueueNode;
  }
  currentNode->nextQueueNode = newNode;
  return 2;
}

Process* deQueueProcess(ProcessQueue* queue) {
  if (queue == NULL || queue->head == NULL) return NULL;
  
  QueueNode* currentNode = queue->head;
  queue->head = queue->head->nextQueueNode;
  Process* process = currentNode->process;
  free(currentNode);
  return process;
}

void displayProcessNode(ProcessNode* node) {
  printf("(%d, %d)", node->CPUTime, node->IORequest);
}

void displayProcess(Process* process) {
  printf("\nPID: %d\nName: %s\n[", process->PID, process->processName);
  ProcessNode* currentNode = process->firstNode;
  while (currentNode != NULL) {
    displayProcessNode(currentNode);
    currentNode = currentNode->nextNode;
  }
  printf("]\n");
}

void displayProcessQueue(ProcessQueue* queue) {
  printf("Queue:\n\n");
  QueueNode* currentNode = queue->head;
  int i = 0;
  while(currentNode != NULL) {
    printf("[%d]:\n", i);
    displayProcess(currentNode->process);
    currentNode = currentNode->nextQueueNode;
    printf("\n");
  }
  printf("End of queue.\n\n");
}








