/*
 * inode.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef INODE_H
#define INODE_H

#define HDA_SECTORSIZE 256
#define NB_BLOC (HDA_SECTORSIZE - 4*sizeof(int))/sizeof(int)


enum file_type_e {
  AFILE, ADIRECTORY
};

struct inode_s {
	enum file_type_e type;
	int size;
	int nb_bloc[NB_BLOC];
	int indirect1;
	int indirect2;
};

void read_inode (unsigned int inumber, struct inode_s *inode);

void write_inode (unsigned int inumber, struct inode_s *inode);

unsigned int create_inode (enum file_type_e type);

int delete_inode (unsigned int inumber);

#endif
