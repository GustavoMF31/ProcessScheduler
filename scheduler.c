#include <stdlib.h>

#include "scheduler.h"
#include "process.h"
#include "options.h"

SchedulerState initialState(EventQueue *q){
  SchedulerState s;
  s.executing = NULL;
  s.eventQueue = q;

  s.highPriority = createProcessQueue();
  s.lowPriority = createProcessQueue();

  return s;
}

// Atempts to load the next process to execute from the highest
// priority queue available
void loadNextProcess(SchedulerState *state, SchedulingOptions opt, int time){
  state->executing = deQueueProcess(state->highPriority);
  if (state->executing == NULL) state->executing = deQueueProcess(state->lowPriority);
  if (state->executing == NULL) return;

  state->executing->timeSlicesUsed++;

  // Remember to preempt this process once its time slice is over
  insertIntoEventQueue(state->eventQueue,
      timeSliceFinished(state->executing->PID, state->executing->timeSlicesUsed),
      time + opt.timeSlice);

  printf("Process %d has started executing\n", state->executing->PID);
}

// Advances execution of the current process by one time unit,
// returning true if no work was done
bool stepProcessExecution(SchedulerState *state, SchedulingOptions opt, int time){
  // If there is no currently executing process, there is nothing to be done
  if (state->executing == NULL) return true;

  int remainingTime = --state->executing->firstNode->CPUTime;
  if (remainingTime > 0) return false;

  // In case the current node has finished its CPU time,
  // it should start its next IO action
  IOType io = state->executing->firstNode->IORequest;
  if (io != NONE){
    printf("Process %d has triggered %s IO\n", state->executing->PID, ioNameAsString(io));

    Event e = ioFinished(state->executing, io);
    insertIntoEventQueue(state->eventQueue, e, time + getIODuration(opt, io));

    // Advance the process node chain, so that we start from the
    // next node when this process returns from IO
    state->executing->firstNode = state->executing->firstNode->nextNode;
  } else {
    printf("Process %d has finished execution\n", state->executing->PID);
  }

  // Now we try to bring the next process to execute
  loadNextProcess(state, opt, time);

  return false;
}

// Processes, if necessary, the next event in the EventQueue.
// Returns true if there are no more events and false otherwise.
bool handleNextEvents(SchedulerState *state, SchedulingOptions opt, int time){

  while (true) {
    Event event;
    int eventTime;
    if (!nextEvent(&event, &eventTime, state->eventQueue)) return true;
    if (time < eventTime) return false;

    switch (event.type) {
      case NEW_PROCESS:
        printf("New process with PID %d\n", event.newProcess->PID);
        enQueueProcess(event.newProcess, state->highPriority);
        if (state->executing == NULL) loadNextProcess(state, opt, time);
        break;
      case TIME_SLICE_FINISHED:
        if (state->executing == NULL) break;
        if (state->executing->PID != event.processToPreempt) break;
        if (state->executing->timeSlicesUsed != event.timeSlicesUsed) break;
        printf("Preempted process %d\n", state->executing->PID);
        enQueueProcess(state->executing, state->lowPriority);
        loadNextProcess(state, opt, time);
        break;
      case IO_FINISHED:
        if (event.blockedProcess->firstNode == NULL){
          printf("Process %d has finished execution\n", event.blockedProcess->PID);
          break;
        }
        ProcessQueue* q = hasPriority(event.ioType) ? state->highPriority : state->lowPriority;
        enQueueProcess(event.blockedProcess, q);
        printf("Process %d finished its IO\n", event.blockedProcess->PID);
        break;
      default:
        printf("Invalid event type found in handleNextEvent\n");
        exit(1);
    }

    dropEvent(state->eventQueue);
  }
}

bool schedulingStep(SchedulerState *state, SchedulingOptions opt, int time){
  bool done = stepProcessExecution(state, opt, time);
  done = handleNextEvents(state, opt, time) && done;

  return done;
}
