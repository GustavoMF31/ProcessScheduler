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

// Attempts to read the next event from the queue. Returns false if the queue
// is empty and true otherwise, writing the event and its time to
// the supplied pointers
bool nextEvent(Event *event, int *time, EventQueue* queue);

// Removes the next event from the queue, if it exists
void dropEvent(EventQueue* queue);

bool newProcessEventsFromFile(FILE* file, EventQueue* e);
