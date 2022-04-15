#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void write_(int count, char last) {
    fwrite(&count, sizeof(int), 1, stdout); // write 4-bit integer address to file
    printf("%c", last);                     // write char to file
}

int main (int argc, char *argv[]) {
    // needs at least 1 argument
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]");
        exit(1);
    }

    bool first  = false;    // boolean to test if first byte has been read yet
    int count   = 0;        // char counter
    char chr;               // "actual" current char in string, null term used as default value because it will never occur in file
    char last;              // "current" char that is being counted -- is compared to chr_current

    
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
    
        if (!fp) {
            printf("wzip: unable to open file\n");
            exit(1);
        }
            
        while(chr = fgetc(fp) != EOF) {
            // initial condition:
            // for the first character, there is 
            // no previous character to refer to,
            // we must handle it independently 
            if (!first) {
                last    = chr;
                count   = 1;
                first   = true;
            }

            // add to count of current character
            else if (last == chr) {
                ++count;
            }

            // if character changes, make it the new
            // character to be compared to the others
            else {
                write_(count, last);
                last    = chr;
                count   = 1;
            }
        }
        fclose(fp);
    }
    write_(count, last);
    return 0;
}