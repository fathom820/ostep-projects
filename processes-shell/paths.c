#include <dirent.h>         // opendir()
#include <unistd.h>         // access()
#include <linux/limits.h>   // PATH_MAX
#include <string.h>
#include <stdio.h>

#include "paths.h"

const char *cwd[PATH_MAX];

// PRIVATE //
void paths_init_cwd() {
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       //printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }
}

void paths_run(char **args, char *path) {
    //paths_init_cwd(); // set *cwd to current working directory so it can be reset at the end
    DIR *p = opendir(path);
    //chdir(path); // change directory to path
    char full[256]; // create empty string
    strcpy(full, path); // add
    strcat(full, args[0]);

    if (access(full, F_OK) == 0) {
        // file exists
        int pid = fork();

        //args[1][strcspn(args[1], "\r\n")] = 0;

        if (pid == 0) {
            execv(full, args);
            printf("paths.c:31 : this shouldn't print\n");
        }
        

    } else {
        // file doens't exist
    }

    //chdir(cwd); // change directory back to normal (executable directory)
}