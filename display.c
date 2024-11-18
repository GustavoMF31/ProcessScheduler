#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "event.h"
#include "eventqueue.h"
#include "options.h"
#include "process.h"
#include "scheduler.h"
#include "display.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// Displays a process node
void displayProcessNode(ProcessNode* node) {
  printf("(%d, %d)", node->CPUTime, node->IORequest);
}

// Displays all information about a process
void displayProcess(Process* process) {
  if (process == NULL) {
    printf("---");
    return;
  }
  printf("P[%d]: [", process->PID);
  ProcessNode* currentNode = process->firstNode;
  while (currentNode != NULL) {
    displayProcessNode(currentNode);
    currentNode = currentNode->nextNode;
  }
  printf("]");
}

// Displays a process queue
void displayProcessQueue(ProcessQueue* queue) {
  QueueNode* currentNode = queue->head;
  if (currentNode == NULL) {
    printf("     --- ");
    return;
  }
  int i = 0;
  while(currentNode != NULL) {
    printf("     (%d).\n     ", i);
    displayProcess(currentNode->process);
    currentNode = currentNode->nextQueueNode;
    printf("\n\n");
    i++;
  }
}

// Prints a line of chosen length and character
void charLine(char c, int n) {
  printf(BLUE "\n");
  for (int i = 0; i < n; i++) {
    printf("%c", c);
  }
  printf("\n" RESET);
}

// Displays the list of all processes executing a given type of IO at a given time
void displayIOList(IOType io, SchedulerState* state, int currentTime) {
  // Prints the type of IO being searched
  switch (io) {
    case DISK:
      printf("     Disk:\n\n");
      break;
    case TAPE:
      printf("     Tape:\n\n");
      break;
    case PRINTER:
      printf("     Printer:\n\n");
      break;
    default:
      printf("Error: Invalid type of IO passed to displayIOList\n");
  }

  // Searches through the event queue of current scheduler state for IO_FINISHED events corresponding
  // to the given IO type yet to occur (which means the process is doing IO at the current time)
  EventQueue* e = state->eventQueue;
  CurrentEvent* currentNode = e->firstEvent;

  // In case there is no process executing this type of I/O show an empty list
  bool isEmpty = true; 
  while (currentNode != NULL) {
    if (currentNode->event.type == IO_FINISHED && currentNode->event.ioType == io && currentNode->eventTime > currentTime) {
      printf("     ");
      displayProcess(currentNode->event.blockedProcess);
      printf("\n\n");
      isEmpty = false;
    } 
    currentNode = currentNode->nextEvent; 
  }
  if (isEmpty) printf("     ---\n");
}

// Displays current state of the scheduler in a formated way
// Shows current time, description of the current executing process,
// describes process in high and low prioritiy queues
// and shows all the processes that are currently executing each type of I/O
void displaySchedulerState(SchedulerState* state, int currentTime) {
  // Clears the terminal
  system("clear");

  // Size of each separating line
  int n = 50;

  // Displays current time
  printf("\n\n");
  charLine('=', n);
  printf("\n     Time:" CYAN  " %d\n" RESET, currentTime);
  charLine('_', n);

  // Displays executing process
  printf("\n     Executing: \n\n     " GREEN);
  displayProcess(state->executing);
  printf(RESET);

  // Displays high priority queue
  charLine('_', n);
  printf("\n     High priority queue:\n\n" YELLOW); 
  displayProcessQueue(state->highPriority);
  printf(RESET);

  // Displays low priority queue
  charLine('_', n);
  printf("\n     Low priority queue:\n\n" MAGENTA); 
  displayProcessQueue(state->lowPriority);
  printf(RESET);

  // Displays processes doing each type of IO
  charLine('_', n);
  printf("\n     Doing I/O:\n\n");

  printf(CYAN);
  displayIOList(DISK, state, currentTime);
  printf(RESET);
  
  printf("\n");

  printf(RED);
  displayIOList(TAPE, state, currentTime);
  printf(RESET);

  printf("\n");

  printf(YELLOW);
  displayIOList(PRINTER, state, currentTime);
  printf(RESET);

  printf("\n");
  charLine('=', n);
}
