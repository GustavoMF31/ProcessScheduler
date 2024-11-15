#include <stdlib.h>

typedef struct _CurrentEvent {
  struct _CurrentEvent* nextEvent;
  Event* event;
  int eventDuration;
} CurrentEvent;

typedef struct {
  CurrentEvent* firstEvent;
} EventQueue;

EventQueue* initEventQueue() {
  EventQueue* newEventQueue = (EventQueue*) malloc(sizeof(EventQueue));
  newEventQueue->firstEvent = NULL;
  return newEventQueue;
}

void insertIntoEventQueue(EventQueue* curEventQueue) {
  CurrentEvent* newEvent = (CurrentEvent*) malloc(sizeof(CurrentEvent));
  newEvent->nextEvent = NULL;
  if (curEventQueue->firstEvent == NULL) {
    curEventQueue->firstEvent = newEvent;
  } else {
    CurrentEvent* aux = curEventQueue->firstEvent;
    while(aux->nextEvent != NULL) {
      aux = aux->nextEvent;
    }
    aux->nextEvent = newEvent;
  }
}
