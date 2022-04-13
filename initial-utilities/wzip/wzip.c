#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void write_(int count, char last) {
    fwrite(&count, sizeof(int), 1, stdout);
    printf("%c", last);
}

int main (int argc, char *argv[]) {
    // needs at least 1 argument
    if (argc <= 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int count = 0;                  // char counter
    char chr_current = '\0';        // current char in string, null term used as default value because it will never occur in file
    char chr_last = chr_current;        // "current" char that is being counted
    
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
    
        if (!fp) {
            printf("wzip: unable to open file\n");
            exit(1);
        }
    
        while(1) {
            chr_current = fgetc(fp);
            // exit at end of file
            if (chr_current == EOF) {
                break;
            }

            // initial condition
            if (chr_last == '\0') {
                chr_last = chr_current;
                count = 1;
            }

            // add to count of current character
            else if (chr_last == chr_current) {
                ++count;
            }

            // if character changes
            else {
                write_(count, chr_last);
                chr_last = chr_current;
                count = 1;
            }
        }
        fclose(fp);
    }

    write_(count, chr_last);
    return 0;
}