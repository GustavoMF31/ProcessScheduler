#include <stdio.h>

#include "event.h"
#include "eventqueue.c"

int main(){
  // Quickly test some of the implemented functionality
  EventQueue* e = initEventQueue();

  insertIntoEventQueue(e, ioFinished(1, DISK), 20);
  insertIntoEventQueue(e, timeSliceFinished(2, 1), 10);

  printf("IO: %d\n", e->firstEvent->event.blockedProcess);
  printf("TimeSlice: %d\n", e->firstEvent->nextEvent->event.processToPreempt);
  return 0;
}
