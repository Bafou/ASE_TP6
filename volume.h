/*
 * volume.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef VOLUME_H
#define VOLUME_H

#define MAGIC_SB 0xb00b5
#define MAGIC_FREE 0x5aa55
#define MAX_TAILLE 32
#define HDA_SECTORSIZE 256
#define NB_BLOC (HDA_SECTORSIZE - 4*sizeof(int))/sizeof(int)

unsigned int current_vol;
struct superbloc_s superbloc;

enum file_type_e {
  AFILE, ADIRECTORY
};

struct superbloc_s {
  int magic;
  unsigned int serialn; // serial number
  char name[MAX_TAILLE];
  unsigned int first_free_bloc;
  unsigned int root;
  int nb_free;
};

struct free_bloc_s {
	unsigned int next;
  int magic;
};

struct inode_s {
	enum file_type_e type;
	int size;
	int nb_bloc[NB_BLOC];
	int indirect1;
	int indirect2;
};

void read_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer);

void write_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer);

void frmt_bloc(unsigned int vol, unsigned int bloc, unsigned int value);

void read_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);

void write_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);

void load_super(unsigned int vol);

void save_super();

void init_super(unsigned int vol, char* name);

#endif
