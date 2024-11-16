#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "event.h"

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

EventQueue* initEventQueue();
void insertIntoEventQueue(EventQueue* curEventQueue, Event event, int time);

bool newProcessEventsFromFile(FILE* file, EventQueue* e);
