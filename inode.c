/*
 * inode.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "hw_config.h"
#include "Driver.h"
#include "volume.h"
#include "mbr.h"
#include "alloc.h"
#include "inode.h"

void read_inode (unsigned int inumber, struct inode_s *inode) {
  if (inumber >= mbr.vol[current_vol].size || inumber < 0) {
    fprintf(stderr, "Error: Le inumber %d n'est pas un numéro d'inoeud\n", inumber);
    exit(EXIT_FAILURE);
  }
  read_blocn(current_vol, inumber, (unsigned char*) inode,sizeof(struct inode_s));
}
// TODO finir cette fonction
void write_inode (unsigned int inumber, struct inode_s *inode) {

}

unsigned int create_inode (enum file_type_e type) {
  struct inode_s inode;
  int allocated;
  allocated = new_bloc();
  if (allocated < 0) {
    fprintf(stderr, "Impossible de créer un nouvel inoeud.\n");
    exit(EXIT_FAILURE);
  }
  inode.type = type;
  inode.size = 0;
  inode. // je sais pas quoi
  // je sais pas quoi
  write_blocn(current_vol, allocated, (unsigned char*) &inode,sizeof(struct inode_s));
  return allocated;
}

int delete_inode (unsigned int inumber) {
  // libérer tous les bloc du fichier
  return free_bloc(inumber); // puis à la toute fin, libérer inoeud
}
