/*
 * ifile.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef IFILE_H
#define IFILE_H

#define HDA_SECTORSIZE 256

struct file_desc_s {
	unsigned int inumber;
  int size;
  int offset;
  int dirty;
  char buffer[HDA_SECTORSIZE];
};

unsigned int create_ifile(enum file_type_e type);

int delete_ifile(unsigned int inumber);

int open_ifile(struct file_desc_s * fd, unsigned int inumber);

void close_ifile(struct file_desc_s * fd);

void flush_ifile(struct file_desc_s * fd);

void seek_ifile(struct file_desc_s * fd, int r_offset);

int readc_ifile(struct file_desc_s * fd);

int writec_ifile(struct file_desc_s * fd, char c);

#endif
