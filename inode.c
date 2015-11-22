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
    fprintf(stderr, "Erreur 'read_inode' : inode corrompu.\n");
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
    fprintf(stderr, "Erreur 'create_inode' : Impossible de créer un nouvel inoeud.\n");
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
    if (inode.direct[i]) {
      free_bloc(inode.direct[i]);
    }
  }

  // indirect blocks
  if (inode.indirect) {
    read_bloc(current_vol, inode.indirect, buf1);
    for (i = 0; i < NB_ENTRIES_INDIRECT; i++) {
      if (buf1[i]) {
        free_bloc(buf1[i]);
      }
    }
    free_bloc(inode.indirect);
  }

  // indirect_double blocks
  if (inode.indirect_double) {
    read_bloc(current_vol, inode.indirect_double, buf1);
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
    free_bloc(inode.indirect_double);
  }

  free_bloc(inumber);
  return 1;
}

unsigned int read_indirect(unsigned int ind, unsigned int fbloc, int do_allocate) {
  int entries_direct[HDA_SECTORSIZE];
  read_bloc(current_vol, ind, (unsigned char *) entries_direct);
  if (entries_direct[fbloc] == BLOC_NULL && do_allocate) {
    entries_direct[fbloc] = new_bloc();
    if (entries_direct[fbloc] == -1) {
      fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
      return BLOC_NULL;
    }
    write_bloc(current_vol, ind, entries_direct);
  }
  return entries_direct[fbloc];
}

unsigned int read_indirect_double(unsigned int ind_double, unsigned int fbloc, int do_allocate) {
  int entries_indirect[HDA_SECTORSIZE];
  unsigned int res;
  // load first indirection in entries_indirect
  read_bloc(current_vol, ind_double, (unsigned char *) entries_indirect);
  if (entries_indirect[fbloc/NB_ENTRIES_INDIRECT] == BLOC_NULL) {
    if (!do_allocate){
      return BLOC_NULL;
    }
    else {
      entries_indirect[fbloc/NB_ENTRIES_INDIRECT] = new_bloc();
      if (entries_indirect[fbloc/NB_ENTRIES_INDIRECT] == - 1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return BLOC_NULL;
      }
      write_bloc(current_vol, ind_double, (unsigned char *) entries_indirect);
    }
  }
  // load second indirection in entries_indirect
  res = entries_indirect[fbloc/NB_ENTRIES_INDIRECT];
  read_bloc(current_vol, res, (unsigned char *) entries_indirect);
  if (entries_indirect[fbloc%NB_ENTRIES_INDIRECT] == BLOC_NULL) {
    if (!do_allocate) {
      return BLOC_NULL;
    }
    else {
      entries_indirect[fbloc%NB_ENTRIES_INDIRECT] = new_bloc();
      if (entries_indirect[fbloc%NB_ENTRIES_INDIRECT] == - 1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return BLOC_NULL;
      }
      write_bloc(current_vol, res, (unsigned char *) entries_indirect);
    }
  }
  return entries_indirect[fbloc%NB_ENTRIES_INDIRECT];
}

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, int do_allocate) {
  struct inode_s inode;
  //int entries_indirect[HDA_SECTORSIZE];
  read_inode(inumber, &inode);
  if (fbloc < NB_ENTRIES_DIRECT) { // searching in direct entries
    if ((inode.direct[fbloc] == BLOC_NULL) && do_allocate) {
      inode.direct[fbloc] = new_bloc();
      if (inode.direct[fbloc] == -1) {
        fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
        return BLOC_NULL;
      }
      write_inode(inumber, &inode);
    }
    return inode.direct[fbloc];
  }
  else if (fbloc < (NB_ENTRIES_DIRECT + NB_ENTRIES_INDIRECT)) { // searching in indirect entries
    if (inode.indirect == BLOC_NULL) {
      if (!do_allocate){
        return BLOC_NULL;
      }
      else {
        inode.indirect = new_bloc();
        if (inode.indirect_double == -1) {
          fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
          return BLOC_NULL;
        }
        write_inode(inumber, &inode);
      }
    }
    return read_indirect(inode.indirect, fbloc - NB_ENTRIES_DIRECT,do_allocate);
  }
  else if (fbloc < NB_ENTRIES_TOTAL) { // searching in indirect double entries
    if (inode.indirect_double == BLOC_NULL) {
      if (!do_allocate){
        return BLOC_NULL;
      }
      else {
        inode.indirect_double = new_bloc();
        if (inode.indirect_double == -1) {
          fprintf(stderr, "Error: Impossible to allocate a new bloc, out of memory.\n");
          return BLOC_NULL;
        }
        write_inode(inumber, &inode);
      }
    }
    return read_indirect_double(inode.indirect_double, fbloc-(NB_ENTRIES_DIRECT+NB_ENTRIES_INDIRECT), do_allocate);
  }
  return BLOC_NULL;
}
