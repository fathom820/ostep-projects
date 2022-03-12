#include <stdlib.h>     // other shit
#include <string.h>     // string ops
#include <stdio.h>      // io
#include <unistd.h>     // chdir

#include "builtins.h"
#include "error.h"

int builtins_run(char **args) {
    int found = 0;

    char *cmd = args[0];
    cmd[strcspn(cmd, "\r\n")] = 0;  // remove newline chars for both DOS and UNIX formats, since WSL likes to cause problems :/

    if (strcmp(cmd, "exit") == 0) {
        found = 1;
        if (args[1] != NULL) { // can't have any arguments, exit is a standalone command
            cmd_error("An error has occurred\n");
        } else {
            exit(EXIT_SUCCESS);
        }
        printf("builtins.c[19]: this shouldn't print\n"); // Why did I need to write this? it's a long story...
    }
    if (strcmp(cmd, "cd") == 0) {
        found = 1;
        // if no arguments
        if (args[1] == NULL) {
            cmd_error("An error has occurred\n");
        }
        
        // if 1 argument (proper case)
        if (args[1] != NULL) {
            chdir(args[1]);
        }

        // 2 or more arguments
        if (args[2] != NULL) {
            //printf(args[2]);
            cmd_error("An error has occurred\n");
        }
    }
    if (strcmp(cmd, "path") == 0) {
        
    }

    return found;
}