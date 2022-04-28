#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define THRESHOLD 0 // when file bigger than this size, multithreading will be used

// ! assumes file has already been opened
void write_(int count, char last) {
  fwrite(&count, sizeof(int), 1, stdout); // write 4-bit integer address to stdout
  printf("%c", last);                     // write char to stdout
}

void output_add(char *s, int ct, char c) {
  char temp[strlen(s) + sizeof(int) + sizeof(char)]; // allocate string size of s with room for another entry
  // for ()
}

typedef struct __arg_val {
  int start;  // index to start at
  int end;    // index to end at
  char *in;   // mmap
} arg_val_t;

typedef struct __ret_val {
  char *out;
} ret_val_t;

typedef struct __arg {
  arg_val_t arg_val; // passed to worker thread
  ret_val_t ret_val; // passed back to main thread
} arg_t;

void *worker(void *arg) {
  arg_t *newarg = (arg_t*) arg;
  int start = newarg->arg_val.start;// printf("start: %d\n", start);
  int end   = newarg->arg_val.end; //printf("end: %d\n", end);
  char *in  = newarg->arg_val.in;
  char *out = newarg->ret_val.out; //printf("out: %s\n", out);

  int count = 0;
  char chr_current;
  char chr_last;

  
  for (int i = start; i <= end; i++) {
    // * special case for very first character
    chr_current = in[i];

    if (i == 0) {
      chr_last = chr_current;
      count = 1;
    }
    // * add to count of current character
    else if (chr_last == chr_current) {
      ++count;
    }
    // * if character changes, make it the new
    // * character to be compared to the others
    else {
      //TODO: write
      // fwrite(&count,sizeof(int), 1, stdout);
      char str[100];
      sprintf(str, "%ls", &count);
      printf("%d", &count);
      printf("%c", chr_last);

      chr_last = chr_current;
      count = 1;
    }
  }
  printf("\n");
  return NULL;
}

  
// * initialize an argument struct to be passed to worker
void arg_init(arg_t *arg, int start, int end, char *in, char *out) {
  arg->arg_val.start  = start;  //printf("arg_init: %d\n", start);
  arg->arg_val.end    = end;    //printf("arg_init: %d\n", end);
  arg->arg_val.in     = in;     //printf("arg_init: %s\n", in);
  arg->ret_val.out    = out;    //printf("arg_init: %s\n\n", out);
}

int main (int argc, char *argv[]) {
  if (argc == 1) {
    printf("wzip: file1 [file2 ...]\n");
    exit(1);
  }


  // * var init
  struct stat st;     // stat for measuring file size 
  int file_size;      // size of current file 

  // ? might need to refactor (move) this block 
  bool first_read = false; // if first byte has been read (special case)
  int count = 0;           // counter for chr_last
  char chr_current;        // current char being compared to chr_last
  char chr_last;           // current char being counted


  // * for each file ... 
  for (int i = 1; i < argc; i++) {
    // * open file, test if valid; exit program if not
    FILE *fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wzip: unable to open file\n");
      exit(1);
    }

    // * get file size
    stat(argv[i], &st);
    file_size = st.st_size;

    // * determine compression algorithm based on file size
    if (file_size <= THRESHOLD) { // ! p2s1 compression (non-threaded)

      while(1) {
        // * move to next char, exit at EOF
        if ((chr_current = fgetc(fp)) == EOF) break;

        // * initial condition:
        // for the first character, there is
        // no previous character to refer to;
        // we must handle it independently
        if (!first_read) {
          chr_last = chr_current;
          count = 1;
          first_read = true;
        }

        // * add to count of current character
        else if (chr_last == chr_current) {
          ++count;
        }

        // * if character changes, make it the new
        // * character to be compared to the others
        else {
          write_(count, chr_last);
          chr_last = chr_current;
          count = 1;
        }
      }
      fclose(fp);

    } else { // ! p2s2 compression (multi-threaded)
      // * split file (mapped to memory in *src) into three partitions
      // * partitions are evenly sized
      int fd = fileno(fp);            // convert FILE struct to file descriptor
      char *src = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
      int part_size = file_size / 3;  // partition size
                                      // first partition starts at src[0]
      int off1 = part_size;           // second partition starts at src[off1]
      int off2 = off1 * 2;            // third partition starts at src[off2]

      // * arg declaration
      arg_t *arg1 = malloc(sizeof(*arg1)); 
      arg_t *arg2 = malloc(sizeof(*arg2));
      arg_t *arg3 = malloc(sizeof(*arg3));

      // * arg initialization
      // point to eventual string output 
      // of each worker thread;
      // initialized to "null" to avoid segfaults
      char *arg1_out = "null";
      char *arg2_out = "null";
      char *arg3_out = "null";

      // * initialize arg structs for each function
      // so they can be passed through pthread_create()
      arg_init(arg1, 0, off1, src, arg1_out);         // arg1: goes from 0 to just before off1;     output stored in arg1_out
      arg_init(arg2, off1, off2, src, arg2_out);      // arg2: goes from off1 to just before off2;  output stored in arg2_out
      arg_init(arg3, off2, file_size, src, arg3_out); // arg3: goes from off2 to EOF;               output stored in arg3_out
      
      // * threading (fun)
      pthread_t t;
      pthread_create(&t, NULL, worker, (void *) arg1); // first partition
      pthread_create(&t, NULL, worker, (void *) arg2); // second partition
      pthread_create(&t, NULL, worker, (void *) arg3); // third partition

      pthread_join(t, NULL);

    }
  }
  // The whole thing breaks if I change this and
  // I'm not brave enough to try and figure out why.
  if (file_size <= THRESHOLD) write_(count, chr_last);
  return 0;
}