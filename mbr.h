/*
 * mbr.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef MBR_H
#define MBR_H
#define MAX_VOL 8
#define MAGIC 0x55aa

enum type_e {
  BASE,
  ANNEXE,
  OTHER
};

struct descriptor_s {
  unsigned int cylinder;
  unsigned int sector;
  unsigned int size;
  enum type_e type;
};

struct mbr_s {
  int magic;
  unsigned nb_vol;
  struct descriptor_s vol[MAX_VOL];
};

extern struct mbr_s mbr;

int load_mbr();

void save_mbr();

#endif
