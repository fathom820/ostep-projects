#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#include "cmdops.h" // command operations
#include "builtins.h" // builtin commands
#include "paths.h" // attack on titan reference lol

/** @deprecated */
char **cmd_args;
extern char *path = "/bin/";

int main (int argc, char *argv[]) {
    
    // runs batch mode (! tests use this mode !)
    if (argc == 2) {
        FILE *fp = fopen(argv[1], "r"); 

        if (fp == NULL) {
            perror("fopen");
            return 1;
        }

        char *line = NULL;
        size_t len = 0;
        size_t read;

        while ((read = getline(&line, &len, fp)) != -1) {
            //printf("%s", line);
            line[strcspn(line, "\r\n")] = 0; // remove newline
            cmd_args = cmdops_split_line(line);
            char *cmd = cmd_args[0];

            // built-ins
            builtins_run(cmd_args);

            // paths
            paths_run(cmd_args, path);
        }

        fclose(fp);
        if(line) {
            free(line);
        }
        return 0;

    }
    // runs interactive mode 
    else if (argc == 1) {
        // todo
    }
    // if too many arguments
    else {
        puts("Max arguments: 1");
    }

}