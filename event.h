#pragma once

#include "process.h"

// Event types, ordered according to tie-breaking priority
typedef enum {
  NEW_PROCESS,
  IO_FINISHED,
  TIME_SLICE_FINISHED
} EventType;

// Temporal events the scheduler has to keep track of
typedef struct _Event {
  /* Depending on the type of the event, different fields of this
     Event structure are considered valid to be written and read.*/
  EventType type;

  // Fields for type NEW_PROCESS:
  Process* newProcess;

  // Fields for type TIME_SLICE_FINISHED:

  /* Process ID of the process that has consumed its full time slice at the time
     of the occurrence of this event */
  int processToPreempt;

  /* How many time slices the process has used so far. Useful to make sure the
     event refers to the correct time the process has been scheduled for execution */
  int timeSlicesUsed;

  // Fields for type IO_FINISHED:

  // The process that was blocked by an IO request
  Process* blockedProcess;

  /* The type of IO that has ended. Used to make sure the process returns to the process queue
     of the right priority */
  IOType ioType;
} Event;

Event newProcess(Process *p);
Event timeSliceFinished(int pid, int timeSlicesUsed);
Event ioFinished(Process *p, IOType type);

// Returns true if e1 should come before e2 in case of a tie in time
bool compareEvent(Event e1, Event e2);

// Returns whether events of type t1 have priority over events of type
// t2 in case of a tie on event time
bool compareEventType(EventType t1, EventType t2);
