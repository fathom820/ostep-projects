#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cmdops.h"
#include "error.h"

#define BUFSIZE 256

extern int cmd_args_len;

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

char **cmdops_get_redirect(char *line) {                // note that *line is modified by this function

    char **out = malloc(2 * sizeof(char*));             // 2 element array
    int bufsize = BUFSIZE;
    int pos = 0;
    int redir_count = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '>') {
            redir_count++;
            break;
        }
    }

    out[0] = strtok(line, ">");
    out[1] = strtok(NULL, ">");

    if (out[1] == NULL && redir_count == 1) {           // check if redirection requested, but no file specified

        error("An error has occurred\n");
        exit(0);
    } else if (out[1] != NULL && redir_count == 1) {

        for (int i = 0; i < strlen(out[1]); i++) {      // check for multiple output files

            if (out[1][i] == ' ') {
                error("An error has occurred\n");
                exit(0);
            }
        }

        FILE *fp = fopen(out[1], "w+");

        if (!fp) {
            printf("%s\n", "cmd_ops_get_redirect : fucked up");
        }
    }
    
    return out;
}

/**
 * @brief Basically a strtok() wrapper. Splits a given string into an array of strings,
 * separated by each occurence of *delim.
 * 
 * @param line line to split
 * @param delim character to split line by
 * @param count number of 
 * @return char** array of strings
 */
char **cmdops_split(char *line, char *delim) {
    cmd_args_len = 0;
    int bufsize = BUFSIZE;
    int pos = 0;

    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "cmdops_split: allocation error 1\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, delim);
    
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        cmd_args_len++;

        // resize array if necessary (90% of the time)
        if (pos >= bufsize) {
            bufsize += BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "cmdops_split: allocation error 2\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    
    tokens[pos] = NULL;
    return tokens;
}