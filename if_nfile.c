#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h"
#include "ifile.h"

int main(int argc, char** argv) {

  unsigned int inumber;
  char c;
  struct file_desc_s fd;

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
