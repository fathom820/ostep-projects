#include <dirent.h>         // opendir()
#include <unistd.h>         // access()
#include <linux/limits.h>   // PATH_MAX
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "paths.h"
#include "cmdops.h"


char path[256][256] = {"/bin"};
int path_len = 1;


void paths_set(char *newpath[256], int len) {
    for (int i = 0; i < len; i++) {
        strcpy(path[i], newpath[i]);
    }
    path_len = len;
}

int paths_run(char **args) {
    int found = 0;

    if (path_len == 0) {
        return found; // don't waste time searching if there's no path to search lol
    }

    int i = 0;
    while(found == 0 && i < path_len) {
        char full[2048];                // I commend anyone who is able to type in a command that's over 2048 characters long.
        strcpy(full, path[i]);
        strcat(full, "/");
        strcat(full, args[0]);

        if (access(full, R_OK) == 0) {
            found = 1;

            int pid = fork();
            if (pid == 0) {
                //TODO: redirect_verify();
                execv(full, args);
                perror("execv");
            } else {
                pid = wait(NULL);
            }   
        }
        i++;
    }
  
    return found;
}