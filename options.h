#pragma once

#include <stdbool.h>
#include "process.h"

typedef struct {
  // How long is each time slice, i.e, how much time is given to each process to
  // execute before they are sent back to the "ready" queue
  int timeSlice;

  // How long it takes, in "time units", to complete IO of each type
  int printerTime;
  int diskTime;
  int tapeTime;
  bool showSteps;
} SchedulingOptions;

bool parseArgs(SchedulingOptions* opt, int argc, char** argv);
int getIODuration(SchedulingOptions opt, IOType io);
