/*
 * inode.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw_config.h"
#include "Driver.h"
#include "volume.h"
#include "mbr.h"
#include "alloc.h"
#include "inode.h"

void read_inode (unsigned int inumber, struct inode_s * inode) {
  read_blocn(current_vol, inumber, (unsigned char*) inode,sizeof(struct inode_s));
  if (inode->magic != MAGIC_INODE) {
    inode = NULL;
    fprintf(stderr, "Error 'read_inode' : inode corrompu.\n");
    exit(EXIT_FAILURE);
  }
}

void write_inode (unsigned int inumber, struct inode_s *inode) {
  struct inode_s buf;
  read_inode(inumber, &buf);
  write_blocn(current_vol, inumber, (unsigned char*) inode,sizeof(struct inode_s));
}

unsigned int create_inode (enum file_type_e type) {
  struct inode_s inode;
  int allocated;
  memset(&inode,0,sizeof(struct inode_s));
  inode.type = type;
  allocated = new_bloc();
  if (allocated < 0) {
    fprintf(stderr, "Impossible de créer un nouvel inoeud.\n");
    return 0;
  }
  inode.magic = MAGIC_INODE;
  write_blocn(current_vol, allocated, (unsigned char*) &inode,sizeof(struct inode_s));
  return allocated;
}

int delete_inode (unsigned int inumber) {
  int i,j;
  struct inode_s inode;
  unsigned char buf1[HDA_SECTORSIZE];
  unsigned char buf2[HDA_SECTORSIZE];

  read_inode(inumber,&inode);
  // direct blocks
  for (i = 0; i < NB_ENTRIES_DIRECT; i++){
    if (inode.entries[i]) {
      free_bloc(inode.entries[i]);
    }
  }

  // indirect1 blocks
  if (inode.indirect1) {
    read_bloc(current_vol, inode.indirect1, buf1);
    for (i = 0; i < NB_ENTRIES_INDIRECT; i++) {
      if (buf1[i]) {
        free_bloc(buf1[i]);
      }
    }
    free_bloc(inode.indirect1);
  }

  // indirect2 blocks
  if (inode.indirect2) {
    read_bloc(current_vol, inode.indirect2, buf1);
    for (i = 0; i < NB_ENTRIES_INDIRECT; i++) {
      if (buf1[i]) {
        read_bloc(current_vol, buf1[i], buf2);
        for (j = 0; j < NB_ENTRIES_INDIRECT; i++) {
          if (buf2[j]) {
            free_bloc(buf2[j]);
          }
        }
        free_bloc(buf1[i]);
      }
    }
    free_bloc(inode.indirect2);
  }

  free_bloc(inumber);
  return inumber;
}
