#include <stdio.h>

#include "event.h"
#include "eventqueue.h"
#include "options.h"
#include "process.h"

// Quickly test some of the implemented functionality
void testQueue(){
  EventQueue* e = initEventQueue();

  insertIntoEventQueue(e, ioFinished(1, DISK), 20);
  insertIntoEventQueue(e, timeSliceFinished(2, 1), 10);

  printf("IO: %d\n", e->firstEvent->event.blockedProcess);
  printf("TimeSlice: %d\n", e->firstEvent->nextEvent->event.processToPreempt);
}

// Testing function to display the values of the options set through the
// command line
void dumpOptions(SchedulingOptions opt){
  printf("Time slice: %d\n", opt.timeSlice);
  printf("Disk time: %d\n", getIODuration(opt, DISK));
  printf("Tape time: %d\n", getIODuration(opt, TAPE));
  printf("Printer time: %d\n", getIODuration(opt, PRINTER));
}

int main(int argc, char** argv){
  SchedulingOptions opt;
  if (!parseArgs(&opt, argc, argv)){
    printf("Invalid command line arguments\n");
    return 1;
  }

  // Test reading the process data from a csv file
  // TODO: Get file name from the command line
  FILE* file = fopen("new_process_data.csv", "r");
  if (file == NULL){
    printf("Could not find file with process data\n");
    return 1;
  }

  EventQueue* e = initEventQueue();
  bool ok = newProcessEventsFromFile(file, e);
  if (!ok){
    printf("Failed to read processes from file\n");
    return 1;
  }

  Process *p1 = e->firstEvent->event.newProcess;
  Process *p2 = e->firstEvent->nextEvent->event.newProcess;
  Process *p3 = e->firstEvent->nextEvent->nextEvent->event.newProcess;

  displayProcess(p1);
  displayProcess(p2);
  displayProcess(p3);

  return 0;
}
