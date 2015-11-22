/*
 * mbr.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef MBR_H
#define MBR_H
#define MAX_VOL 8
#define MAGIC 0x55aa

enum vol_type_e {
  BASE,
  ANNEXE,
  OTHER
};

struct volume_s {
  unsigned int cylinder; // begining cylinder
  unsigned int sector; // begining sector
  unsigned int size; // nb of sectors
  enum vol_type_e type; // the of the volume
};

struct mbr_s {
  int magic;
  unsigned nb_vol;
  struct volume_s vol[MAX_VOL];
};

extern struct mbr_s mbr;

int load_mbr();

void save_mbr();

#endif
