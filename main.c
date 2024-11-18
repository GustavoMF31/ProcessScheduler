#include <stdio.h>
#include <stdlib.h>

#include "event.h"
#include "eventqueue.h"
#include "options.h"
#include "process.h"
#include "scheduler.h"
#include "display.h"

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
  fclose(file);

  bool done = false;
  int time = 0;
  SchedulerState state = initialState(e);
  //TODO: Add this option to command line argument logic
  bool displayStatesOption = true;
  while (!done) {
    done = schedulingStep(&state, opt, time);
    if (displayStatesOption) {
      displaySchedulerState(&state, time);
      printf("\n     [Press any key to continue]\n");
      getc(stdin);
    }
    time++;
  }

  // Free event queue and process queues
  free(e);
  free(state.highPriority);
  free(state.lowPriority);

  // printf("Scheduling done in %d units of time\n", time);
  return 0;
}
