#include <stdlib.h>     // other shit
#include <string.h>     // string ops
#include <stdio.h>      // io
#include <unistd.h>     // chdir

#include "builtins.h"
#include "error.h"
#include "paths.h"


extern char *path;
extern int cmd_args_len;


int builtins_run(char **args) {
    int found = 0;

    if (args == NULL) {
        puts("If you can see this, it means the programmer really fucked up.");
    }
    
    char *cmd = args[0];
    if (cmd == NULL) {
        printf("no cmd");
        return found;
    }
    if (strcmp(cmd, "exit") == 0) {
        found = 1;
        if (args[1] != NULL) {      // can't have any arguments, exit is a standalone command
            error();
        } else {
            exit(0);
        }
    }
    if (strcmp(cmd, "cd") == 0) {
        found = 1;
        
        if (args[1] == NULL) {      // if no arguments
            error();
        }
        if (args[1] != NULL) {      // if 1 argument (proper case)
            chdir(args[1]);
        }
        if (args[2] != NULL) {      // 2 or more arguments
            error();
        }
    }
    if (strcmp(cmd, "path") == 0) {
        found = 1;
        char *temp[256];

        for (int i = 1; i < cmd_args_len; i++) {
            temp[i-1] = args[i];
        }
        paths_set(temp, cmd_args_len - 1);
    }
    
    return found;
}