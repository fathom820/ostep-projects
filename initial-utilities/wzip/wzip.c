#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>


// ! assumes file has already been opened
void write_(int count, char last) {
  fwrite(&count, sizeof(int), 1, stdout); // write 4-bit integer address to file
  printf("%c", last);                     // write char to file
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
  // ? to lesser scope inside (file_size <= 4096)
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
    if (file_size <= 4096) { // ! p2s1 compression (non-threaded)

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
      // TODO: shit here
      int fd = fileno(fp); // convert FILE struct to file descriptor (thanks POSIX!)
      char *src = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
      // printf("%ld\n", strlen(src));
      int off1 = file_size / 3;
      int off2 = off1 * 2;
      // printf("%d %d\n", off1, off2);
      printf("%d, %d\n", file_size, off1 * 3);
    }
  }
  // the whole thing breaks if I change this and
  // I'm not brave enough to try and figure out why.
  if (file_size <= 4096) write_(count, chr_last);
  return 0;
}




// ! p2s1 main; i refactored everything for p2s2
// ! keeping this here for
// int mainold (int argc, char *argv[]) {
//   // needs at least 1 argument
//   switch (argc) {
//     case 1:
//       printf("wzip: file1 [file2 ...]\n");
//       exit(1);

//     default:
//       {   
//         bool first = false;     // boolean to test if first byte has been read yet
//         int count = 0;          // char counter
//         char chr_current;       // current char in string, null term used as default value because it will never occur in file
//         char chr_last;          // "current" char that is being counted -- is compared to chr_current

//         for (int i = 1; i < argc; i++) {
//           FILE *fp = fopen(argv[i], "r");

//           if (!fp) {
//             printf("wzip: unable to open file\n");
//             exit(1);
//           }

//           while(1) {
//             chr_current = fgetc(fp);

//             // exit at end of file
//             if (chr_current == EOF) {
//               break;
//             }

//             // initial condition:
//             // for the first character, there is 
//             // no previous character to refer to,
//             // we must handle it independently 
//             if (!first) {
//               chr_last = chr_current;
//               count = 1;
//               first = true;
//             }

//             // add to count of current character
//             else if (chr_last == chr_current) {
//               ++count;
//             }

//             // if character changes, make it the new
//             // character to be compared to the others
//             else {
//               write_(count, chr_last);
//               chr_last = chr_current;
//               count = 1;
//             }
//           }
//           fclose(fp);
//         }
//         write_(count, chr_last);
//         return 0;
//       }
//   }
// }
