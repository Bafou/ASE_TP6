/*
 * ifile.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "ifile.h"
#include "inode.h"
#include "volume.h"

unsigned int create_ifile(enum file_type_e type){
  return create_inode(type);
}

int delete_ifile(unsigned int inumber){
  return delete_inode(inumber);
}

int open_ifile(struct file_desc_s * fd, unsigned int inumber){
  struct inode_s;
  memset(fd,0,sizeof(struct file_desc_s));
  read_inode(inumber,&inode);
  fd->inumber = inumber;
  fd->size = inode.size;
  return 1;
}

void close_ifile(struct file_desc_s * fd){
  flush_ifile(fd);
  return;
}

void flush_ifile(struct file_desc_s * fd){
  if (fd->dirty) {
    write_bloc(current_vol, vbloc_of_fbloc(fd->inumber,(fd->offset/HDA_SECTORSIZE),fd->buffer));
    fd->dirty = 0;
  }
}

void seek_ifile(struct file_desc_s * fd, int r_offset){

}

int readc_ifile(struct file_desc_s * fd){
  //if (fd.offset)
}

int writec_ifile(struct file_desc_s * fd, char c){

}
