#include <stdlib.h>

#include "scheduler.h"
#include "process.h"
#include "options.h"

SchedulerState initialState(EventQueue *q){
  SchedulerState s;
  s.executing = NULL;
  s.currentNode = NULL;
  s.eventQueue = q;

  s.highPriority = createProcessQueue();
  s.lowPriority = createProcessQueue();
  s.blocked = createProcessQueue();

  return s;
}

// Atempts to load the next process to execute from the highest
// priority queue available
void loadNextProcess(SchedulerState state){
  state.executing = deQueueProcess(state.highPriority);
  if (state.executing == NULL) state.executing = deQueueProcess(state.lowPriority);
  if (state.executing == NULL) return;

  // TODO: Here we assume that the firstNode is always non-null.
  // Does the function that loads processes guarantee this?
  state.currentNode = state.executing->firstNode;
}

// Advances execution of the current process by one time unit,
// returning true if no work was done
bool stepProcessExecution(SchedulerState state, SchedulingOptions opt, int time){
  // If there is no currently executing process, there is nothing to be done
  if (state.executing == NULL) return true;

  int remainingTime = --state.currentNode->CPUTime;
  if (remainingTime > 0) return false;

  // In case the current node has finished its CPU time,
  // it should start its next IO action
  if (state.currentNode->IORequest != NONE){
    IOType io = state.currentNode->IORequest;
    Event e = ioFinished(state.executing, io);
    insertIntoEventQueue(state.eventQueue, e, time + getIODuration(opt, io));
    enQueueProcess(state.executing, state.blocked);

    // Advance the process node chain, so that we start from the
    // next node when this process returns from IO
    state.executing->firstNode = state.executing->firstNode->nextNode;
  }

  // Now we try to bring the next process to execute
  loadNextProcess(state);

  // Advance the process node if the current one has finished
  state.currentNode = state.currentNode->nextNode;

  return false;
}

// Processes, if necessary, the next event in the EventQueue.
// Returns true if there are no more events and false otherwise.
bool handleNextEvent(SchedulerState state, int time){
  Event event;
  int eventTime;
  if (!nextEvent(&event, &eventTime, state.eventQueue)) return true;
  if (time < eventTime) return false;

  switch (event.type) {
    case NEW_PROCESS:
      enQueueProcess(event.newProcess, state.highPriority);
      break;
    case TIME_SLICE_FINISHED:
      // TODO: Ensure this preemption refers to the correct time
      // this process has been scheduled for execution
      if (state.executing->PID != event.processToPreempt) break;
      enQueueProcess(state.executing, state.lowPriority);
      loadNextProcess(state);
      break;
    case IO_FINISHED:
      ProcessQueue* q = hasPriority(event.ioType) ? state.highPriority : state.lowPriority;
      enQueueProcess(event.blockedProcess, q);
      break;
    default:
      printf("Invalid event type found in handleNextEvent\n");
      exit(1);
  }

  dropEvent(state.eventQueue);
  return false;
}

bool schedulingStep(SchedulerState state, SchedulingOptions opt, int time){
  bool done = stepProcessExecution(state, opt, time);
  done = done && handleNextEvent(state, time);

  // TODO: Make sure the IO queue is empty before signaling that we are done
  return done;
}
