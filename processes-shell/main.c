#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

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
            cmd_error("An error has occurred\n");
            exit(1);
        }

        char *line = NULL;
        size_t len = 0;
        size_t read;

        while ((read = getline(&line, &len, fp)) != -1) {
            line[strcspn(line, "\r\n")] = 0; // remove newline (better safe than sorry)
            
            char **temp = cmdops_get_redirect(line);
            
            cmd_args = cmdops_split_line(temp[0]);
            
            
            output_file = temp[1];
            
            FILE *op = fopen(output_file, "w+");
            
            
            if (op) {
                fclose(op);
                //printf("test");
                //int out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                //printf("%d\n", out);
                //dup2(out, 1);
                //close(out);
            }
       
            // if a command was issued & wasn't a built in command or found in path, throw error
            if (cmd_args_len > 0 && !builtins_run(cmd_args) && !paths_run(cmd_args)) {
                cmd_error();
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
        // TODO: interactivem ode
    }

    // if too many arguments
    else {
        cmd_error();
        exit(1);
    }
}