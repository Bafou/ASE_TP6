#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ifile.h"

int main(int argc, char** argv) {

  unsigned int inumber1, inumber2;
  char c;
  struct file_desc_s fd1, fd2;
  if (argc != 2) {
    fprintf(stderr, "[%s] usage:\n\t"
            "%s inumber1 inumber2\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }
  inumber1 = strtol(argv[1], NULL, 10);
  inumber2 = strtol(argv[1], NULL, 10);
  mount();
  open_ifile(&fd1,inumber1);
  open_ifile(&fd2,inumber2);
  while((c=readc_ifile(&fd1)) != -1){
    writec_ifile(&fd2,c);
  }
  close_ifile(&fd1);
  close_ifile(&fd2);
  umount();
  return 0;
}
