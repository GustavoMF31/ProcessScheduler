#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "event.h"

// Attempts to read a positive integer from argv at index i+1, preceeded by the argument name argName.
// Returns true if succeeded, writing the result to *dest, and false otherwise.
bool readArg(char* argName, int* dest, int argc, char** argv, int i){

  // If the argName does not match or we have run out of input, return false
  if (strcmp(argv[i], argName) != 0) return false;
  if (i == argc-1) return false;

  // Perform the reading otherwise
  return sscanf(argv[i+1], "%d", dest) == 1 && *dest > 0;
}

// Reads the scheduling options from the command line arguments and writes them
// into *opt. Returns true upon success and false otherwise.
bool parseArgs(SchedulingOptions *opt, int argc, char** argv) {

  // Set default values for the options
  opt->timeSlice = 5;
  opt->diskTime = 4;
  opt->tapeTime = 7;
  opt->printerTime = 9;

  // Iterate over the arguments pairwise, expecing "--option value" pairs
  for (int i = 0; i < argc; i += 2){
    bool ok = false;

    ok = ok || readArg("--time"   , &opt->timeSlice  , argc, argv, i);
    ok = ok || readArg("--disk"   , &opt->diskTime   , argc, argv, i);
    ok = ok || readArg("--tape"   , &opt->tapeTime   , argc, argv, i);
    ok = ok || readArg("--printer", &opt->printerTime, argc, argv, i);

    // In case of an unrecognized argument, return false;
    if (!ok) return false;
  }

  return true;
}

// Returns how long an IO operation of a specific type takes, according
// to the passed SchedulingOptions. Assumes io != None.
int getIODuration(SchedulingOptions opt, IOType io){
  switch (io) {
    case PRINTER: return opt.printerTime;
    case DISK: return opt.diskTime;
    case TAPE: return opt.tapeTime;
    default:
      printf("Invalid IOType passed to getIODuration\n");
      exit(1);
  }
}
