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
  open_ifile(&fd,inumber);
  while((c=readc_ifile(&fd)) != -1)
    putchar(c);

  close_ifile(&fd);
  umount();
  return 0;
}
