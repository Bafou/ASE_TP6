/*
 * ifile.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "ifile.h"
 #include "inode.h"

unsigned int create_ifile(enum file_type_e type){
  return create_inode(type);
}

int delete_ifile(unsigned int inumber){
  return delete_inode(inumber);
}

int open_ifile(file_desc_s * fd,unsigned int inumber){

}

void close_ifile(file_desc_s * fd){

}

void flush_ifile(file_desc_s * fd){

}

void seek_ifile(file_desc_s * fd, int r_offset){

}

int readc_ifile(file_desc_s * fd){

}

int writec_ifile(file_desc_s * fd, char c){

}