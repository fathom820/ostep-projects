#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void cmd_error() {
  fprintf(stderr, "%s\n", "An error has occurred");
}
