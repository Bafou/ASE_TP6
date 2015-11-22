/*
 * inode.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef INODE_H
#define INODE_H

#define HDA_SECTORSIZE 256
#define MAGIC_INODE 0xc1cad0
#define NB_ENTRIES_DIRECT (HDA_SECTORSIZE - 5*sizeof(int))/sizeof(int)
#define NB_ENTRIES_INDIRECT HDA_SECTORSIZE/sizeof(int)
#define NB_ENTRIES_TOTAL (NB_ENTRIES_DIRECT + NB_ENTRIES_INDIRECT + NB_ENTRIES_INDIRECT*NB_ENTRIES_INDIRECT)
#define BLOC_NULL 0

enum file_type_e {
  AFILE,
  ADIRECTORY
};

struct inode_s {
	enum file_type_e type;
  int magic;
	int size;
	int direct[NB_ENTRIES_DIRECT];
	int indirect;
	int indirect_double;
};

void read_inode (unsigned int inumber, struct inode_s *inode);

void write_inode (unsigned int inumber, struct inode_s *inode);

unsigned int create_inode (enum file_type_e type);

int delete_inode (unsigned int inumber);

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, int do_allocate);

#endif
