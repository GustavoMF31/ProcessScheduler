#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "event.h"

// Constructor functions to create each kind of event

Event newProcess(Process *p){
  Event e;
  e.type = NEW_PROCESS;
  e.newProcess = p;

  return e;
}

Event timeSliceFinished(int pid, int timeSlicesUsed){
  Event e;
  e.type = TIME_SLICE_FINISHED;
  e.processToPreempt = pid;
  e.timeSlicesUsed = timeSlicesUsed;

  return e;
}

Event ioFinished(Process *p, IOType type){
  Event e;
  e.type = IO_FINISHED;
  e.blockedProcess = p;
  e.ioType = type;

  return e;
}

bool compareEvent(Event e1, Event e2){
  if (e1.type != e2.type) return compareEventType(e1.type, e2.type);
  switch (e1.type) {
    case NEW_PROCESS: return e1.newProcess->PID < e2.newProcess->PID;
    case IO_FINISHED: return e1.blockedProcess->PID < e2.blockedProcess->PID;

    // Ordering among TIME_SLICE_FINISHED events turns out not to matter for
    // purposes of scheduling
    case TIME_SLICE_FINISHED: return true;
    default:
      printf("Invalid event types in compareEvent\n");
      exit(1);
  }
}

bool compareEventType(EventType t1, EventType t2){
  // Here we use the fact that in the enum declaration, the types
  // are ordered according to tie-breaking priority
  return t1 < t2;
}
