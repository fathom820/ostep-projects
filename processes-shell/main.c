#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdops.h" // command operations
#include "builtins.h" // builtin commands
#include "paths.h" // attack on titan reference lol
#include "error.h" // cmd_error()

char **cmd_args;
int cmd_args_len;

char *output_file;


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
            line[strcspn(line, "\r\n")] = 0; // remove newline
            
            char **temp = cmdops_get_redirect(line);
            cmd_args = cmdops_split_line(temp[0]);
            output_file = temp[1];
            char *cmd = cmd_args[0];
            
            if (!builtins_run(cmd_args)) {
                if(!paths_run(cmd_args)) {
                    cmd_error("An error has occurred\n");
                }
            }
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