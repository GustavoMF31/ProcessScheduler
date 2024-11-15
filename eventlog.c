#include <stdio.h>
#include <stdlib.h>

typedef struct {
} Event;

typedef struct _CurrentEvent{
  struct _CurrentEvent* NextEvent;
  Event* event;
  int EventDuration;
} CurrentEvent;

typedef struct {
  CurrentEvent* FirstEvent;
} EventLog;

EventLog* initEventLog() {
  EventLog* NewEventLog = (EventLog*) malloc(sizeof(EventLog));
  NewEventLog->FirstEvent = NULL;
  return NewEventLog;
}

void insertIntoEventLog(EventLog* CurEventLog) {
  CurrentEvent* NewEvent = (CurrentEvent*) malloc(sizeof(CurrentEvent));
  NewEvent->NextEvent = NULL;
  if (CurEventLog->FirstEvent == NULL) {
    CurEventLog->FirstEvent = NewEvent;
  } else {
    CurrentEvent* aux = CurEventLog->FirstEvent;
    while(aux->NextEvent != NULL) {
      aux = aux->NextEvent;
    }
    aux->NextEvent = NewEvent;
  }
}