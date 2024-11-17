#pragma once

#include "process.h"
#include "eventqueue.h"
#include "options.h"

typedef struct {
  // Currently executing process (which might be NULL)
  Process* executing;

  // Queue with future events
  EventQueue* eventQueue;

  // Queues with "ready" processes
  ProcessQueue* highPriority;
  ProcessQueue* lowPriority;

} SchedulerState;

SchedulerState initialState(EventQueue *q);

// Advances the scheduling by one unit of time returning true when there is
// no more execution to be done. Prints the actions taken to stdout.
bool schedulingStep(SchedulerState *state, SchedulingOptions opt, int time);
