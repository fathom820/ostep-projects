#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BUFSIZE 2006240 

pthread_mutex_t lock;

int thres = 0;

int *arg1_out_cnts;
int *arg2_out_cnts;
int *arg3_out_cnts;
char *arg1_out_chrs;
char *arg2_out_chrs;
char *arg3_out_chrs;

int t1_sz = 0;
int t2_sz = 0;
int t3_sz = 0;

char *final_chrs;
int *final_cnts;
int final_sz = 0;

// ! assumes file has already been opened
void write_(int count, char last) {
  fwrite(&count, sizeof(int), 1, stdout); // write 4-bit integer address to stdout
  printf("%c", last);                     // write char to stdout
}

void thread_cat() {
  bool done = false;
  final_cnts = malloc(BUFSIZE * sizeof(char));
  final_chrs = malloc(BUFSIZE * sizeof(char));

  int i = 0;
  for (int j = 0; j < t1_sz; j++) {
    final_cnts[i] = arg1_out_cnts[j];
    final_chrs[i] = arg1_out_chrs[j];

    // printf("%d%c ", final_cnts[i], final_chrs[i]);
    i++;
    final_sz++;
   }
  for (int j = 0; j < t2_sz; j++) {
    final_cnts[i] = arg2_out_cnts[j];
    final_chrs[i] = arg2_out_chrs[j];

    // printf("%d%c ", final_cnts[i], final_chrs[i]);
    i++;
    final_sz++;
  }
  for(int j = 0; j < t3_sz; j++) {
    final_cnts[i] = arg3_out_cnts[j];
    final_chrs[i] = arg3_out_chrs[j];

    // printf("%d%c ", final_cnts[i], final_chrs[i]);
    i++;
    final_sz++;
  }
  
  for (i = 0; i < final_sz; i++) {
    // printf("%d%c ", final_cnts[i], final_chrs[i]);
  }

  // * repeat n=3 times (max amount of re-occurrences);
  int temp_cnts[final_sz];
  // char final_chrs[final_sz];

  for (int n = 0; n < 3; n++) {
    for (i = 0; i < final_sz; i++) {
      if(i != final_sz - 1) {
        if (final_chrs[i] == final_chrs[i+1]) {
          final_cnts[i] += final_cnts[i+1]; 
          
          for (int j = i+1; j < final_sz-1; j++) {
            final_chrs[j] = final_chrs[j+1];
            final_cnts[j] = final_cnts[j+1];
          }
          final_sz--;
        }
      }   
    }
  }

  for (int i = 0; i < final_sz; i++) {
    printf("%d%c ", final_cnts[i], final_chrs[i]);
  }
}

// * possibly the worst code I've ever written
// (it works though)
// void thread_cat_() { // meow
//   final_chrs = malloc(BUFSIZE * sizeof(char));
//   final_cnts = malloc(BUFSIZE * sizeof(int));

//   bool first_done = false;
//   bool second_done = false;

//   int i;

//   for (i = 0; i < t1_sz-1; i++) {
//     final_chrs[i] = arg1_out_chrs[i];
//     final_cnts[i] = arg1_out_cnts[i];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]);
//     // printf("%d", t1_sz);
//   }
//   // compare last of first thread to first of second
//   if (arg1_out_chrs[i] == arg2_out_chrs[0]) {
//     final_chrs[i] = arg1_out_chrs[i];
//     final_cnts[i] = arg1_out_cnts[i] + arg2_out_cnts[0];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]); 
//     i++; // skip first of second thread since we already added its count
//   } else {
//     final_chrs[i] = arg1_out_chrs[i];
//     final_cnts[i] = arg1_out_cnts[i];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]); 
//   }

//   for (i; i < t2_sz - 1; i++) {
//     final_chrs[i] = arg2_out_chrs[i];
//     final_cnts[i] = arg2_out_cnts[i];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]);

//   }
//   if (arg2_out_chrs[i] == arg3_out_chrs[0]) {
//     final_chrs[i] = arg2_out_chrs[i];
//     final_cnts[i] = arg2_out_cnts[i] + arg3_out_cnts[0];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]);
//     i++;
//   } else {
//     final_chrs[i] = arg2_out_chrs[i];
//     final_cnts[i] = arg2_out_cnts[i];
//     // printf("\n%d%c ", final_cnts[i], final_chrs[i]);
//   }

//   for (i; i < t3_sz; i++) {
//     final_chrs[i] = arg3_out_chrs[i];
//     final_cnts[i] = arg3_out_cnts[i];
//     printf("\n%d%c ", final_cnts[i], final_chrs[i]);
//   }
// }


// I've spent 90% of my time on this project
// trying to get all of these pointers to work.
// Every time I close my eyes, I see asterisks.
// void change(char **a, char *b) {
//   *a = b;
// }

typedef struct __arg_val {
  int start;  // index to start at
  int end;    // index to end at
  char *in;   // mmap
  int mod;    // determines ret arrays
} arg_val_t;


typedef struct __arg {
  arg_val_t arg_val; // passed to worker thread
} arg_t;

// * worker thread
// takes in mapped file contents
// returns char[] with the following format:
//   2a-1b-3c
// this is not the final product, it will be read and processed
// after combining it with every other partition & thread
void *worker(void *arg) {
  // pthread_mutex_lock(&lock);

  arg_t *newarg = (arg_t*) arg;
  int start = newarg->arg_val.start;// printf("start: %d\n", start);
  int end   = newarg->arg_val.end; //printf("end: %d\n", end);
  char *in  = newarg->arg_val.in;
  int mod   = newarg->arg_val.mod;

  if (!mod) { // for debugging, never runs unless i manually change "!mod" to "mod"
    printf("mod: %d; start: %d; end: %d; length: %d;\n", mod, start, end, end-start);
    for (int i = start; i < end; i++) {
      printf("%c", in[i]);
    }
    printf("\n\n");
  }

  int count = 0;
  char chr_current;
  char chr_last;
  bool first_read = false;

  int *out_cnts;  int cnts_i = 0;
  char *out_chrs; int chrs_i = 0;
  
  switch(mod) {
    case 1:
      out_cnts = arg1_out_cnts;
      out_chrs = arg1_out_chrs;
      break;
    case 2:
      out_cnts = arg2_out_cnts;
      out_chrs = arg2_out_chrs;
      break;
    case 3:
      out_cnts = arg3_out_cnts;
      out_chrs = arg3_out_chrs;
      break;
  }

  //    out_cnts = &arg1_out_cnts;
   //   out_chrs = &arg1_out_chrs;
      // printf("%p\n", out_cnts);
      // printf("%p\n", arg1_out_cnts);

  for (int i = start; i <= end; i++) {
    // * special case for very first character
    chr_current = in[i];

    if (!first_read) {
      chr_last = chr_current;
      count = 1;

      first_read = true;
      // printf("first: %c\n", chr_last);
    }

    // * add to count of current character
    else if (chr_last == chr_current && i < end) {
      // printf("here %d\n", count + 1);
      ++count;
      // printf("%c",chr_current);
    }
    // * if character changes, make it the new
    // * character to be compared to the others
    else {
      // if (i == end) ++count;
      // printf("done");
      //TODO: write
      
      *(out_cnts + cnts_i) = count;
      *(out_chrs + chrs_i) = chr_last;  

      // printf("%d%c ", count, chr_last);
      // printf("%d%c ", out_cnts[cnts_i], out_chrs[chrs_i]);
      cnts_i++;
      chrs_i++;

      switch(mod) {
        case 1:
          t1_sz++;
          break;
        case 2:
          t2_sz++;
          break;
        case 3:
          t3_sz++;
          break;
      }
    
      chr_last = chr_current;
      count = 1;
    }
  }
  // printf("\n");
  // pthread_mutex_unlock(&lock);
  return NULL;
}

  
// * initialize an argument struct to be passed to worker
void arg_init(arg_t *arg, int start, int end, char *in, int mod) {
  arg->arg_val.start  = start;  // printf("arg_init: %d\n", start);
  arg->arg_val.end    = end;    // printf("arg_init: %d\n", end);
  arg->arg_val.in     = in;     // printf("arg_init: %s\n", in);
  arg->arg_val.mod    = mod;
}

int main (int argc, char *argv[]) {
  pthread_mutex_init(&lock, NULL);

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
    if (file_size <= thres) { // ! p2s1 compression (non-threaded)

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

      arg1_out_cnts = malloc(BUFSIZE * sizeof(int));
      arg2_out_cnts = malloc(BUFSIZE * sizeof(int));
      arg3_out_cnts = malloc(BUFSIZE * sizeof(int));
      arg1_out_chrs = malloc(BUFSIZE * sizeof(char));
      arg2_out_chrs = malloc(BUFSIZE * sizeof(char));
      arg3_out_chrs = malloc(BUFSIZE * sizeof(char));

      // * initialize arg structs for each function
      // so they can be passed through pthread_create()
      arg_init(arg1, 0, off1, src, 1);             // arg1: goes from 0 to just before off1;
      arg_init(arg2, off1, off2, src, 2);          // arg2: goes from off1 to just before off2;
      arg_init(arg3, off2, file_size, src, 3); // arg3: goes from off2 to EOF;
      
      // * threading
      pthread_t t;
      pthread_create(&t, NULL, worker, (void *) arg1); // first partition
      pthread_create(&t, NULL, worker, (void *) arg2); // second partition
      pthread_create(&t, NULL, worker, (void *) arg3); // third partition

      pthread_join(t, NULL);
      
      // printf("%d", file_size);, out_cnti

      // printf("%d %d %d", t1_sz, t2_sz, t3_sz);

      thread_cat();
    }
  }
  // The whole thing breaks if I change this and
  // I'm not brave enough to try and figure out why.
  if (file_size <= thres) write_(count, chr_last);
  return 0;
}