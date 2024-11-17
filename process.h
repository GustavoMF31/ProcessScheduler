#pragma once

#include <stdbool.h>

// Process handling library 

// Data structures ----------------------------------------------------------------------

// Device types of the I/O requests
typedef enum {
  NONE = 0,
  DISK = 1,
  TAPE = 2,
  PRINTER = 3
} IOType;

// Elemental particle of a process:
// a pair of CPU time and I/O type and a pointer to the next node
typedef struct ProcessNode {
  int CPUTime;
  IOType IORequest;
  struct ProcessNode* nextNode;
} ProcessNode;

// Process ID and pointer to the first node of the chain, as well as how
// many time slices it has used so far
typedef struct Process {
  int PID;
  int timeSlicesUsed;
  ProcessNode* firstNode;
} Process;

// Process queue node
typedef struct QueueNode {
  Process* process;
  struct QueueNode* nextQueueNode;
} QueueNode;

// Process queue
typedef struct ProcessQueue {
  QueueNode* head;
} ProcessQueue;


// Returns whether a process that requested a specific kind of IO
// should return to the high priority queue. Assumes type != NONE,
// as a "NONE" IO cannot be requested.
bool hasPriority(IOType type);

// Spells out the display name for each IOType
char* ioNameAsString(IOType type);

// Process handling functions -----------------------------------------------------------

// Adds a new node to a process and returns:
// 0 if an error occurred
// 1 if the node was the first added to the process
// 2 if the node was added to the end of an existing chain
int addProcessNode(Process *process, int CPUTime, IOType IORequest); 


// Creates an empty process with given PID and name and returns:
// NULL if an error occurred
// a pointer to the new process otherwise
Process* createEmptyProcess(int PID); 

// Creates an empty process queue and returns:
// NULL if an error occurred
// a pointer to the new queue otherwise
ProcessQueue* createProcessQueue();

// Adds a process to the end of an existing queue and returns:
// 0 if an error occurred
// 1 if the process was added to an empty queue
// 2 if the process was added to the end of a non-empty queue 
int enQueueProcess(Process* process, ProcessQueue* queue);

// Removes the head of the process queue and returns:
// NULL if an error occurred
// a pointer to the process removed from the queue otherwise 
Process* deQueueProcess(ProcessQueue* queue);

// Functions for testing ----------------------------------------------------------------

// Displays a single process node
void displayProcessNode(ProcessNode* node);

// Displays all information about a process
void displayProcess(Process* process);

// Displays all the processes in a process queue
void displayProcessQueue(ProcessQueue* queue);
