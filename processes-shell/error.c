#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void cmd_error(char *msg) {
  fprintf(stderr, "%s", msg);
  exit(0);
}
