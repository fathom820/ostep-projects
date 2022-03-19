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

char path[256][256] = {"/bin"};
extern int path_len = 1;

// PRIVATE //
void paths_init_cwd() {
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       //printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
   }
}



void paths_set(char *newpath[256], int len) {
    
    for (int i = 0; i < len; i++) {
        strcpy(path[i], newpath[i]);
    }
    path_len = len;
}

int paths_run(char **args) {
    
    if (path_len == 0) {
        return 0; // don't waste time searching if there's no path to search
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
                execv(full, args);
                printf("this shouldn't print\n");
                perror("execv");
            }
        }
        i++;
    }
  
    /*char full[1024]; // create empty string
    strcpy(full, path[i]); // copy path to string
    strcat(full, "/");
    strcat(full, args[0]); // append filename to path
    
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
    }*/
    return found;
}