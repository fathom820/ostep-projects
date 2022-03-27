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

//char *output_file;


int main (int argc, char *argv[]) {
    // runs batch mode (! tests use this mode !)
    if (argc == 2) {
        FILE *fp = fopen(argv[1], "r"); 

        if (fp == NULL) {
            error("An error has occurred\n");
            exit(1);
        }

        char *line = NULL;
        size_t len = 0;
        size_t read;

        while ((read = getline(&line, &len, fp)) != -1) {
            line[strcspn(line, "\r\n")] = 0; // remove newline (better safe than sorry)
            
            char **commands = cmdops_split(line, "&");
            // test 16
            

            // for each command in line
            int i = 0;
            while (commands[i] != NULL) {

                char *command = commands[i];
                //printf("%s\n", command);
                char **temp = cmdops_get_redirect(command);
                
                char **cmd_args = cmdops_split(temp[0], " ");
                char *output_file = temp[1];
                //printf(output_file);

                FILE *op = fopen(output_file, "w+");
                if (op) {
                    fclose(op);
                    //printf("test");
                    //int out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    //printf("%d\n", out);
                    //dup2(out, 1);
                    //close(out);
                }

                //printf("%d\n", cmd_args_len);
                if (cmd_args_len > 0 && !builtins_run(cmd_args) && !paths_run(cmd_args)) {
                    error();
                }
              //  printf(command);
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