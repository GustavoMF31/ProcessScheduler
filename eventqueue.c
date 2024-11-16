#include <stdlib.h>

#include "eventqueue.h"
#include "event.h"
#include "process.h"

EventQueue* initEventQueue() {
  EventQueue* newEventQueue = (EventQueue*) malloc(sizeof(EventQueue));
  newEventQueue->firstEvent = NULL;
  return newEventQueue;
}

void insertIntoEventQueue(EventQueue* curEventQueue, Event event, int time) {
  CurrentEvent* newEvent = (CurrentEvent*) malloc(sizeof(CurrentEvent));
  newEvent->event = event;
  newEvent->eventTime = time;

  // TODO: Ensure that, when two events happen to have the same eventTime,
  // they are inserted in the EventQueue in the correct order according to
  // some fixed tie-breaking criteria
  CurrentEvent** aux = &curEventQueue->firstEvent;
  while (*aux != NULL && (*aux)->eventTime < time) {
    aux = &(*aux)->nextEvent;
  }
  newEvent->nextEvent = *aux;
  *aux = newEvent;
}

// Reads a line of input in "buffer" and parses it, reading comma-separated
// integers corresponding to cpu times and IO types (PRINTER, TAPE, or
// DISK), building a chain of ProcessNodes that is written to "node".
bool parseNodes(char* buffer, ProcessNode** node){

  while (true){
    // Skip whitespace
    while (buffer[0] == ' ') buffer++;

    // Check if we are done
    if (buffer[0] == '\n' || buffer[0] == '\0') break;

    int cpuTime;
    int charsRead;
    int read = sscanf(buffer, "%d%n", &cpuTime, &charsRead);

    if (read != 1) cpuTime = 0;
    else buffer += charsRead;

    if (buffer[0] == ','){
      buffer++;
      // Skip whitespace
      while (buffer[0] == ' ') buffer++;
    }

    IOType type;
    char* c = strchr(buffer, ',');
    if (c == NULL) c = strchr(buffer, '\n');

    // TODO: Think about how we would like to handle whitespace
    // at the end of a line
    int n = c - buffer; // Distance to the comma
    if (n == 0){
      type = NONE;
    } else if (strncmp(buffer, "PRINTER", n) == 0){
      type = PRINTER;
    } else if (strncmp(buffer, "DISK", n) == 0) {
      type = DISK;
    } else if (strncmp(buffer, "TAPE", n) == 0) {
      type = TAPE;
    } else {
      // Could not read IO type
      printf("Could not read IO type\n");
      return false;
    }

    // Add node to chain
    *node = malloc(sizeof(ProcessNode));
    (*node)->CPUTime = cpuTime;
    (*node)->IORequest = type;

    // Focus on the next pointer
    node = &(*node)->nextNode;

    // Advance the buffer
    buffer = c+1;
  }

  // End the process node chain and return
  *node = NULL;
  return true;
}

// Reads the processes listed in the file, in CSV format, and adds NEW_PROCESS
// events to the EventQueue accordingly. Returns true if succeeded.
bool newProcessEventsFromFile(FILE* file, EventQueue* e){
  int nextPid = 1;

  const int MAX_LINE_LENGTH = 10000;
  char text_buffer[MAX_LINE_LENGTH];

  // Loop through all lines of the file
  while (true) {
    // Attempt to read a line (NULL indicates EOF)
    if (fgets(text_buffer, MAX_LINE_LENGTH, file) == NULL) break;

    // TODO: Read name from file
    Process *p = createEmptyProcess(nextPid++, "A");

    bool ok = parseNodes(text_buffer, &p->firstNode);
    if (!ok) return false;

    // TODO: Read the process entry time from the file
    insertIntoEventQueue(e, newProcess(p), 5);
  }

  return true;
}
