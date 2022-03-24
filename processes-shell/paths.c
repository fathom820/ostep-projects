#include <dirent.h>         // opendir()
#include <unistd.h>         // access()
#include <linux/limits.h>   // PATH_MAX
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "paths.h"
#include "cmdops.h"
#include "redirect.h"


char cwd[PATH_MAX];


char path[256][256] = {"/bin"};
extern int path_len = 1;


void paths_set(char *newpath[256], int len) {
    
    for (int i = 0; i < len; i++) {
        strcpy(path[i], newpath[i]);
    }
    path_len = len;
}

int paths_run(char **args) {
    
    if (path_len == 0) {
        return 0; // don't waste time searching if there's no path to search lol
    }

    int found = 0;
    int i = 0;

    while(found == 0 && i < path_len) {
        
        char full[2048];
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
            }
        }
        i++;
    }
  
    return found;
}