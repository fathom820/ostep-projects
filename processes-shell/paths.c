#include <dirent.h>         // opendir()
#include <unistd.h>         // access()
#include <linux/limits.h>   // PATH_MAX
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "paths.h"
#include "cmdops.h"


char cwd[PATH_MAX];

//char *path = "/bin";

char *path[256] = {"/bin", NULL};

// PRIVATE //
void paths_init_cwd() {
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       //printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
   }
}

void paths_set(char **newpath) {
    int i = 0;
    while (newpath[i] != NULL) {
        path[i] = newpath[i];
    }
}

int paths_run(char **args) {
    
    int found = 0;
    paths_init_cwd(); // set *cwd to current working directory so it can be reset at the end

    char full[1024]; // create empty string
    strcpy(full, path); // copy path to string
    strcat(full, "/");
    strcat(full, args[0]); // append filename to path*/
    
    //printf("%s\n", full);
    
    if (access(full, R_OK) == 0) {
    // file exists
        
        //printf("%s", "found\n");
        found = 1;
        int pid = fork();

        if (pid == 0) {
            execv(full, args);
            printf("paths.c : this shouldn't print\n");
        }

    } else {
        //printf("%s\n", "not found");
    }
    return found;
}