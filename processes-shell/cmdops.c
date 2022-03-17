#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "cmdops.h"
#include "error.h"

#define BUFSIZE 256

extern int cmd_args_len;

/**
 * @brief Get argument count of command passed to shell
 * 
 * @param cmd string to parse
 * @return int number of args (words)
 */
/*int get_arg_count (char *cmd[]) {

    int out = 0;
    for (int i = 0; i < sizeof(cmd); i++) {
        if (cmd[i] == ' ') {
            out++;
        }
    }

    return out;
}*/

/**
 * @brief Dynamically reallocate the size of global var cmd_args
 * 
 * @param ct count of args
 */
/*void realloc_cmd_args(int ct) {
    free(cmd_args);
    //realloc(cmd_args, ct * sizeof(cmd_args));
}*/


/**
 * @brief Allocate memory block to store user input as string. If user exceeds it (somehow), then reallocate memory to fit.
 * Trims newline characters. cmdops_split_line will be run in tandem with this function and will split the line into tokens
 * using ' ' as a delimiter.
 * 
 * @param line line to read
 * @return char* 
 */
char *cmdops_read_line(char line[]) {
    int pos = 0;
    int bufsize = BUFSIZE;
    char *buf = malloc(sizeof(char) * bufsize);
    char c;

    if (!buf) {
        fprintf(stderr, "cmdops_read_line: allocation error 1\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = line[pos];

        if (c == EOF | c == '\n') {
            buf[pos] = '\0';        // end string output
            return buf;
        } else {
            buf[pos] = c;
        }
        pos++;
    
        if (pos >= bufsize) {
            bufsize += BUFSIZE;
            buf = realloc(buf, bufsize);

            if (!buf) {
                fprintf(stderr, "cmdops_read_line: allocation error 2\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **cmdops_get_redirect(char *line) {
    // line is modified
    char **out = malloc(2 * sizeof(char*)); // 2 element array
    int bufsize = BUFSIZE;
    int pos = 0;

    out[0] = strtok(line, ">");
    //printf(out[0]);
    out[1] = strtok(NULL, ">");
    //printf(out[1]);


    return out;
}

char **cmdops_split_line(char *line) {
    cmd_args_len = 0;
    int bufsize = BUFSIZE;
    int pos = 0;

    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "cmdops_split_line: allocation error 1\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, " ");
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        cmd_args_len++;

        // resize array if necessary (90% of the time)
        if (pos >= bufsize) {
            bufsize += BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "cmdops_split_line: allocation error 2\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " ");
    }
    tokens[pos] = NULL;
    return tokens;
}