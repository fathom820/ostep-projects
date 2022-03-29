#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cmdops.h"     // command operations
#include "builtins.h"   // builtin commands
#include "paths.h"      // attack on titan reference lol
#include "error.h"      // error()


char **cmd_args;
int cmd_args_len;


int main (int argc, char *argv[]) {
    // runs batch mode (! tests use this mode !)
    if (argc == 2) {
        FILE *fp = fopen(argv[1], "r"); 

        if (fp == NULL) {
            error();
            exit(1);
        }


        char *line = NULL;
        size_t len = 0;
        size_t read;

        while ((read = getline(&line, &len, fp)) != -1) {
            line[strcspn(line, "\r\n")] = 0;            // remove newline
            char **commands = cmdops_split(line, "&");  // if more than one cmd per line, split them into array

            int i = 0;
            
            while (commands[i] != NULL) {
                char *command = commands[i];
                //printf("%s\n", commands[1]);
                char **temp = cmdops_get_redirect(command);
                char **cmd_args = cmdops_split(temp[0], " ");
                char *output_file = temp[1];
                
                FILE *op = fopen(output_file, "w+");
                if (op) {
                    fclose(op);
                    //printf("test");
                    //int out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    //printf("%d\n", out);
                    //dup2(out, 1);
                    //close(out);
                }

                if (cmd_args_len > 0 && !builtins_run(cmd_args) && !paths_run(cmd_args)) {
                    error();
                }
                i++;
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
        // TODO: interactive mode
    }

    // if too many arguments
    else {
        error();
        exit(1);
    }
}