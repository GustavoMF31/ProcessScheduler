#pragma once

#include "process.h"
#include "eventqueue.h"
#include "options.h"

typedef struct {
  // Currently executing process (which might be NULL)
  Process* executing;
  // And its currently executing node
  // TODO: Do we really need this varible?
  ProcessNode* currentNode;

  // Queue with future events
  EventQueue* eventQueue;

  // Queues with "ready" processes
  ProcessQueue* highPriority;
  ProcessQueue* lowPriority;

  // Queue with currently blocked processes
  // TODO: Check if we will need this "blocked" queue
  ProcessQueue* blocked;
} SchedulerState;

SchedulerState initialState(EventQueue *q);

// Advances the scheduling by one unit of time returning true when there is
// no more execution to be done
// TODO: The scheduler is supposed to warn the caller about the actions taken,
// so that they can be printed to the user
bool schedulingStep(SchedulerState state, SchedulingOptions opt, int time);
