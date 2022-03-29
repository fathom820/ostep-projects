#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cmdops.h"
#include "error.h"

#define BUFSIZE 256

extern int cmd_args_len;

void remove_spaces(char *s) {
    char *d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

/**
 * @brief Allocate memory block to store user input as string. 
 * If user exceeds it (somehow), then reallocate memory to fit.
 * Trims newline characters. cmdops_split will be run in tandem with this 
 * function and will split the line into tokens
 * using " " as a delimiter.
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

/**
 * @brief It works. Don't touch it.
 * 
 * @param line line to get redirection from
 * @return char** line split into format {command, redirection}
 */
char **cmdops_get_redirect(char *line) {                // note that *line is modified by this function

    char **out = malloc(2 * sizeof(char*));             // 2 element array
    int bufsize = BUFSIZE;
    int pos = 0;
    int redir_count = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '>') {
            redir_count++;
        }
    }
    //printf("%s %d\n\n", line, redir_count);

    if (redir_count > 1) {
        error();
        exit(0);
    }

    out[0] = strtok(line, ">");
    char *redir = out[1] = strtok(NULL, ">");
    redir = strtok(redir, " ");
    char *second = strtok(NULL, " ");
    if (second != NULL) {
        error();
        exit(0);
    }
    


    if (redir == NULL && redir_count == 1) {         
        error();
        exit(0);
    } else if (redir != NULL && redir_count == 1) {

        for (int i = 0; i < strlen(redir); i++) {      // check for multiple output files

            if (redir[i] == ' ') {
                remove_spaces(redir);
            }
        }
        
        FILE *fp = fopen(redir, "w+");

        if (!fp) {
            printf("%s%s%s\n", "oops", redir, "oops");
        }
    }
    
    return out;
}

/**
 * @brief Basically a strtok() wrapper. Splits a given string into an array of strings,
 * separated by each occurence of *delim. Kind of the most important function in the whole program.
 * If there's a bug, 70% chance its happening here.
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