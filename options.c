#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "event.h"

// Attempts to read a positive integer from argv at index i+1, preceeded by the argument name argName.
// Returns the number of read arguments if succeeded, writing the result to *dest, and 0 otherwise.
int readPosIntArg(char* argName, int* dest, int argc, char** argv, int i){

  // If the argName does not match or we have run out of input, return false
  if (strcmp(argv[i], argName) != 0) return 0;
  if (i == argc-1) return 0;

  // Perform the reading otherwise
  if (sscanf(argv[i+1], "%d", dest) == 1 && *dest > 0) return 2;
  else return 0;
}

// Attemps to read a specific argument name from the i-th position of argv.
// Returns whether the argument read matches the wanted argument (1 or 0).
int readBoolArg(char* argName, bool* dest, int argc, char** argv, int i) {
  return *dest = strcmp(argv[i], argName) == 0;
} 

// Reads the scheduling options from the command line arguments and writes them
// into *opt. Returns true upon success and false otherwise.
bool parseArgs(SchedulingOptions *opt, int argc, char** argv) {

  // Set default values for the options
  opt->timeSlice = 5;
  opt->diskTime = 4;
  opt->tapeTime = 7;
  opt->printerTime = 9;
  opt->showSteps = false;

  // Iterate over the arguments, expecting "--option value" or "--option" 
  for (int i = 0; i < argc;){

    int j = i; 

    i += readPosIntArg("--time"   , &opt->timeSlice  , argc, argv, i);
    i += readPosIntArg("--disk"   , &opt->diskTime   , argc, argv, i); 
    i += readPosIntArg("--tape"   , &opt->tapeTime   , argc, argv, i); 
    i += readPosIntArg("--printer", &opt->printerTime, argc, argv, i);
    i +=   readBoolArg("--steps"  , &opt->showSteps  , argc, argv, i); 

    if (i == j) return false;
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
