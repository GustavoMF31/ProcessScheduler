#pragma once

#include "event.h"
#include "eventqueue.h"
#include "options.h"
#include "process.h"
#include "scheduler.h"

// Help function that prints a line of chosen character and length 
void charLine(char c, int n); 

// Displays a single process node
void displayProcessNode(ProcessNode* node);

// Displays all information about a process
void displayProcess(Process* process);

// Displays all the processes in a process queue
void displayProcessQueue(ProcessQueue* queue);

// Displays the list of all processes executing a given type of IO at a given time
void displayIOList(IOType io, SchedulerState* state, int currentTime);

// Displays current state of the scheduler in a formated way
// Shows current time, description of the current executing process,
// describes process in high and low prioritiy queues
// and shows all the processes that are currently executing each type of I/O
void displaySchedulerState(SchedulerState* state, int currentTime);
