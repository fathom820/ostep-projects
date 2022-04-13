#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]){

  if(!(argc > 1)){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    char c = '\0';
    char end = '\0';
    int char_count = 0;
    
    for(int i = 1; i < argc; i++){
        FILE *fp = fopen(argv[i], "r");
        while((c = fgetc(fp)) != EOF){
            if(end == '\0'){
                end = c;
                char_count = 1;
            }
            else if (end == c){
                ++char_count;
            }
            else {
                fwrite(&char_count, sizeof(int), 1, stdout);
                printf("%c", end);
                end = c;
                char_count = 2;
            }
        }  
        fclose(fp); 
    }
    
    fwrite(&char_count, sizeof(int), 1, stdout);
    printf("%c", end);
    return 0;
}