#include <stdio.h>

#include "event.h"
#include "eventqueue.h"
#include "options.h"
#include "process.h"
#include "scheduler.h"

// Quickly test some of the implemented functionality
void testQueue(){
  EventQueue* e = initEventQueue();

  Process *p = createEmptyProcess(5);
  insertIntoEventQueue(e, ioFinished(p, DISK), 20);
  insertIntoEventQueue(e, timeSliceFinished(2, 1), 10);

  printf("IO: %d\n", e->firstEvent->event.blockedProcess->PID);
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

// TODO: Ensure all malloc'd memory is properly freed
int main(int argc, char** argv){
  if (argc <= 1){
    printf("Error: process data file should be passed as the first argument\n");
    return 1;
  }

  char* processDataFile = argv[1];

  SchedulingOptions opt;
  // Skip the program name and the file name when looking for
  // scheduling options
  if (!parseArgs(&opt, argc-2, argv+2)){
    printf("Invalid command line arguments\n");
    return 1;
  }

  // Test reading the process data from a csv file
  FILE* file = fopen(processDataFile, "r");
  if (file == NULL){
    printf("Could not open file %s\n", processDataFile);
    return 1;
  }

  EventQueue* e = initEventQueue();
  bool ok = newProcessEventsFromFile(file, e);
  if (!ok){
    printf("Could not parse processes from file\n");
    return 1;
  }

  bool done = false;
  int time = 0;
  SchedulerState state = initialState(e);
  while (!done) {
    done = schedulingStep(&state, opt, time);
    time++;
  }

  // printf("Scheduling done in %d units of time\n", time);
  return 0;
}
