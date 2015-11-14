/*
 * inode.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "Driver.h"
#include "volume.h"
#include "mbr.h"
#include "hw_config.h"
#include "inode.h"

void read_inode (unsigned int inumber, struct inode_s *inode) {
  printf("%d\n", current_vol);
}

void write_inode (unsigned int inumber, struct inode_s *inode) {

}

unsigned int create_inode (enum file_type_e type) {
  return 1;
}

int delete_inode (unsigned int inumber) {
  return 1;
}
