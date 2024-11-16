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
