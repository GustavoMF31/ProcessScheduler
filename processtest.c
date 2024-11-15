#include <stdio.h>
#include <stdlib.h>
#include "processes.h"

// Testing functionalities 
int main() {

  Process* p = createEmptyProcess(1, "processinho");
  displayProcess(p);
  addProcessNode(p, 12, Printer);
  displayProcess(p);
  addProcessNode(p, 7, None);
  displayProcess(p);
  displayProcessNode(p->firstNode);
  printf("\n");

  ProcessQueue* q = createProcessQueue();
  printf("\nfila criada\n");
  enQueueProcess(p, q);
  printf("\n processo p na fila\n");
  displayProcessQueue(q);
  Process* p2 = createEmptyProcess(3, "banana process");
  addProcessNode(p2, 0, Tape);
  addProcessNode(p2, 10, Disk);
  addProcessNode(p2, 5, None);
  enQueueProcess(p2, q);
  displayProcessQueue(q);
  Process* saiu = deQueueProcess(q);
  displayProcess(saiu);
  displayProcessQueue(q); 
}


