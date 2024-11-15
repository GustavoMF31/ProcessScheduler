#include <stdlib.h>

typedef struct _CurrentEvent {
  struct _CurrentEvent* nextEvent;
  Event event;

  // Time, in "time units" since the start of the scheduling process,
  // when this event is goint to occur
  int eventTime;
} CurrentEvent;

typedef struct {
  CurrentEvent* firstEvent;
} EventQueue;

EventQueue* initEventQueue() {
  EventQueue* newEventQueue = (EventQueue*) malloc(sizeof(EventQueue));
  newEventQueue->firstEvent = NULL;
  return newEventQueue;
}

void insertIntoEventQueue(EventQueue* curEventQueue, Event event, int time) {
  CurrentEvent* newEvent = (CurrentEvent*) malloc(sizeof(CurrentEvent));
  newEvent->nextEvent = NULL;
  newEvent->event = event;
  newEvent->eventTime = time;

  if (curEventQueue->firstEvent == NULL) {
    curEventQueue->firstEvent = newEvent;
    return;
  }

  // TODO: Ensure that, when two events happen to have the same eventTime,
  // they are inserted in the EventQueue in the correct order according to
  // some fixed tie-breaking criteria
  CurrentEvent* aux = curEventQueue->firstEvent;
  while (aux->nextEvent != NULL && aux->nextEvent->eventTime < time) {
    aux = aux->nextEvent;
  }
  aux->nextEvent = newEvent;
}
