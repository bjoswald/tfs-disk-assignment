#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "io.h"

void printc(char c) {
  // your code here
  write(STDOUT_FILENO, &c, sizeof(char));
}

void prints(char* s) {
  write(STDOUT_FILENO, s, strlen(s));
}

void println() {
  printc('\n');
}

void inputs(char dst[], int size) {
  // your code here
 read(STDIN_FILENO,dst,size);



 char* s = strstr(dst, "\n");      // search for new line char

 if (s != NULL)                     // if successful then s now points at the new line
 {
     dst[s-dst] = '\0';
 }                                  // index of newlineChar in buff can be found by pointer subtraction
 else
 {
     dst[size - 1] = '\0';
 }
 
}