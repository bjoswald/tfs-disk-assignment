#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "tfs.h"
#include "io.h"

#define WHITESPACE " \t\r\n"

int main() {

  rootNameHolderLoc = 1;//starting location of root file and directory names
  rootLocationHolder = 11; //starting location of root file and directory location pointers

  int done = 0;
  char* tfs_file = NULL;

  do {

    // prompt is current open file, or none
    char* prompt = tfs_file ? tfs_file : "(no file)";

    // print the prompt, then get a line of input from the user
    prints(prompt);
    prints("> ");
    char line[IN_SIZE];
    inputs(line, IN_SIZE);

    // flag denoting we changed the open TFS, and need to save it to the file
    // after this command has been processed
    int altered = 0;

    char* command = strtok(line, WHITESPACE); // user's command
    char* arg1 = strtok(NULL, WHITESPACE); // first argument
    // TODO: some commands need one more argument
    char* arg2 = strtok(NULL, WHITESPACE);
    if (!strcmp("exit", command)) {
      done = 1;
    }
    else if (!strcmp("display", command)) {
      // most commands don't make sense if there's no open file
      if (!tfs_file) {
        prints("No file open.");
        println();
        continue;
      }
      display(); // display the TFS as columns of hex digits
    }
    else if (!strcmp("create", command)) {
      if (!arg1) {
        prints("Need a filepath.");
        println();
        continue;
      }
      int temp = open(arg1, O_RDONLY);
      if (temp != -1) {
        prints("File already exists: ");
        prints(arg1);
        println();
        close(temp); // close inadvertently opened file
        continue;
      }
      free(tfs_file); // free previous string (if any)
      tfs_file = strdup(arg1); // save path for prompt and writing
      init(); // initialize new TFS
      altered = 1; // flag for saving to file
    }
    else if (!strcmp("open", command)) {
      if (!arg1) {
        prints("Need a filepath.");
        println();
        continue;
      }
      int temp = open(arg1, O_RDONLY);
      if (temp == -1) {
        prints("Error opening file: ");
        prints(arg1);
        println();
        continue;
      }
      // load TFS from new file (previous file already saved)
      read(temp, (char*)tfs, 256);
      close(temp);
      free(tfs_file); // free previous string
      tfs_file = strdup(arg1); // save path for prompt and writing
    }
    // TODO: more commands
    else if (!strcmp("import", command)) {
      if (!tfs_file) {
        prints("No file open.");
        println();
        continue;
      }
      import(arg1,arg2);
    }
     else if (!strcmp("mkdir", command)) {
      if (!tfs_file) {
        prints("No file open.");
        println();
        continue;
      }
      mkdir(arg1); 
    }
    else if (!strcmp("export", command)) {
      if (!tfs_file) {
        prints("No file open.");
        println();
        continue;
      }
      export(arg1,arg2);
    }
    else if (!strcmp("ls", command)) {
      if (!tfs_file) {
        prints("No file open.");
        println();
        continue;
      }
      if (arg1 == NULL){arg1 = "";}
      ls(arg1);
    }
    else {
      prints("Command not recognized: ");
      prints(command);
      println();
    }

    if (altered) {
      // save changes to file
      // TODO: your code here
    }

  } while (!done);

  return 0;
}
