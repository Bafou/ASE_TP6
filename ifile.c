/*
 * ifile.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "ifile.h"
#include "inode.h"
#include "volume.h"

#define DO_ALLOCATE 1
#define DO_NOT_ALLOCATE 0

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
  fd->offset = 0;
  return 1;
}

void close_ifile(struct file_desc_s * fd){
  struct inode_s;
  flush_ifile(fd);
  read_inode(fd->inumber,&inode);
  inode.size = fd->size;
  write_inode(fd->inode,&inode);
  return;
}

void flush_ifile(struct file_desc_s * fd){
  unsigned int bloc_to_read;
  if (fd->dirty) {
    bloc_to_read = vbloc_of_fbloc(fd->inumber,(fd->offset/HDA_SECTORSIZE),DO_ALLOCATE);
    write_bloc(current_vol, bloc_to_read,fd->buffer));
    fd->dirty = 0;
  }
}

void seek_ifile(struct file_desc_s * fd, int r_offset){
  fd->offset += r_offset;
}

int readc_ifile(struct file_desc_s * fd){
  unsigned int bloc_to_read;
  char c;
  if (fd->size <= fd->offset) { // fin fichier
    return -1;
  }
  if (fd->offset % HDA_SECTORSIZE == 0) { // si on est au début ou à la fin du buffer
    flush_ifile(fd);
    bloc_to_read = vbloc_of_fbloc(fd->inumber, (fd->offset) / HDA_SECTORSIZE, DO_NOT_ALLOCATE);
    if (bloc_to_read == BLOC_NULL){
      fprintf(stderr, "Error: Can't read at offset %d\n",(fd->offset) );
      return 0;
    }
    if ((fd->size - (fd->offset)) >= HDA_SECTORSIZE) {
      read_bloc(current_vol, bloc_to_read, fd->buffer);
    }
    else {
      read_blocn(current_vol, bloc_to_read, fd->buffer, (fd->size - (fd->offset)));
    }
  }
  c = fd->buffer[fd->offset % HDA_SECTORSIZE];
  seek_ifile(fd,1);
  return c;
}

int writec_ifile(struct file_desc_s * fd, char c){
  unsigned int bloc_to_read;
  if (fd->offset % HDA_SECTORSIZE == 0) { // si on est au début du buffer
    flush_ifile(fd);
    bloc_to_read = vbloc_of_fbloc(fd->inumber, (fd->offset) / HDA_SECTORSIZE, DO_ALLOCATE);
    if (bloc_to_read == BLOC_NULL){
      fprintf(stderr, "Error: Can't write at offset %d, out of memory\n",(fd->offset) );
      return -1;
    }
    read_bloc(current_vol, bloc_to_read, fd->buffer);
  }
  fd->buffer[fd->offset % HDA_SECTORSIZE] = c;
  fd->dirty = 1;
  seek_ifile(fd,1);
  if (fd->size < fd->offset){
    fd->size = fd->offset;
  }
  return (fd->offset - 1);
}
