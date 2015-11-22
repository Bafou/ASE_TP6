#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h"
#include "ifile.h"

int main(int argc, char** argv) {

  unsigned int inumber;
  char c;
  struct file_desc_s fd;
  if (argc != 2) {
    fprintf(stderr, "[%s] usage:\n\t"
            "%s inumber\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }
  inumber = strtol(argv[1], NULL, 10);
  mount();
  if (delete_ifile(inumber)) printf("ifile %u deleted\n",inumber );;
  umount();

}
