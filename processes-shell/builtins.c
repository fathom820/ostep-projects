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
    if (args == NULL) {
        printf("nigga");
    }
    char *cmd = args[0];
    

    if (cmd == NULL) {
        printf("no cmd");
        return 0;
    }
    int found = 0;
    
    //cmd[strcspn(cmd, "\r\n")] = 0;  // remove newline chars for both DOS and UNIX formats, since WSL likes to cause problems :/
    
    if (strcmp(cmd, "exit") == 0) {
        found = 1;
        if (args[1] != NULL) { // can't have any arguments, exit is a standalone command
            cmd_error();
        } else {
            exit(EXIT_SUCCESS);
        }
    }
    if (strcmp(cmd, "cd") == 0) {
        found = 1;
        // if no arguments
        if (args[1] == NULL) {
            cmd_error();
        }
        
        // if 1 argument (proper case)
        if (args[1] != NULL) {
            chdir(args[1]);
        }

        // 2 or more arguments
        if (args[2] != NULL) {
            cmd_error();
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