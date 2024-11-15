#include "event.h"

// Constructor functions to create each kind of event

// TODO: Event newProcess(?? *processActions)

Event timeSliceFinished(int pid, int timeSlicesUsed){
  Event e;
  e.type = TIME_SLICE_FINISHED;
  e.processToPreempt = pid;
  e.timeSlicesUsed = timeSlicesUsed;

  return e;
}

Event ioFinished(int pid, IOType type){
  Event e;
  e.type = IO_FINISHED;
  e.blockedProcess = pid;
  e.ioType = type;

  return e;
}
