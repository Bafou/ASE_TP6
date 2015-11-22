#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ifile.h"
#include "inode.h"

int main(int argc, char** argv) {

  unsigned int inumber;
  char c;
  struct file_desc_s fd;
  if (argc != 2) {
    fprintf(stderr, "[%s] usage:\n\t"
            "%s\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }
  mount();
  inumber = create_ifile(AFILE);
  printf("fichier d'inode %u créé.\n", inumber);
  open_ifile(&fd,inumber);
  while((c=getchar()) != EOF)
    writec_ifile(&fd,c);

  close_ifile(&fd);
  umount();
  return 0;
}
