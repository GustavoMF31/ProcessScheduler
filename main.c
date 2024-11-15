#include <stdio.h>

#include "event.h"
#include "eventqueue.c"
#include "options.h"

// Quickly test some of the implemented functionality
void testQueue(){
  EventQueue* e = initEventQueue();

  insertIntoEventQueue(e, ioFinished(1, DISK), 20);
  insertIntoEventQueue(e, timeSliceFinished(2, 1), 10);

  printf("IO: %d\n", e->firstEvent->event.blockedProcess);
  printf("TimeSlice: %d\n", e->firstEvent->nextEvent->event.processToPreempt);
}

int main(int argc, char** argv){
  SchedulingOptions opt;
  if (!parseArgs(&opt, argc, argv)){
    printf("Invalid command line arguments\n");
    return 1;
  }

  // Print a few values to check if the parsing was successful
  printf("Time slice: %d\n", opt.timeSlice);
  printf("Disk time: %d\n", getIODuration(opt, DISK));
  printf("Tape time: %d\n", getIODuration(opt, MAGNETIC_TAPE));
  printf("Printer time: %d\n", getIODuration(opt, PRINTER));

  return 0;
}
