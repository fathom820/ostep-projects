#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void cmd_error(char *msg) {
  fprintf(stderr, "%s", msg);
  //exit(0); // causes test 7 to break, might need to be brought back at a later point
}
