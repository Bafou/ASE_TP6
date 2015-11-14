/*
 * volume.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Driver.h"
#include "hw_config.h"
#include "volume.h"
#include "mbr.h"

#define cob cylinder_of_bloc
#define sob sector_of_bloc

unsigned int cylinder_of_bloc(unsigned int vol, unsigned int bloc) {
  assert(mbr.magic == MAGIC);
  if (vol >= mbr.nb_vol || vol < 0) {
    fprintf(stderr, "Error at 'cylinder_of_bloc' : Le volume %d n'existe pas.\n", vol);
    exit(EXIT_FAILURE);
  }
  if (bloc >= mbr.vol[vol].size || bloc < 0) {
    fprintf(stderr, "Error at 'cylinder_of_bloc' : Le bloc %d du volume %d n'existe pas.\n", bloc, vol);
    exit(EXIT_FAILURE);
  }
  return mbr.vol[vol].cylinder + ((bloc + mbr.vol[vol].sector)/HDA_MAXSECTOR);
}

unsigned int sector_of_bloc(unsigned int vol, unsigned int bloc) {
  assert(mbr.magic == MAGIC);
  if (vol >= mbr.nb_vol || vol < 0) {
    fprintf(stderr, "Error at 'cylinder_of_bloc' : Le volume %d n'existe pas.\n", vol);
    exit(EXIT_FAILURE);
  }
  if (bloc >= mbr.vol[vol].size || bloc < 0) {
    fprintf(stderr, "Error at 'cylinder_of_bloc' : Le bloc %d du volume %d n'existe pas.\n", bloc, vol);
    exit(EXIT_FAILURE);
  }

  return ((bloc + mbr.vol[vol].sector)%HDA_MAXSECTOR);
}


void read_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer) {
  read_sector(cob(vol,bloc), sob(vol,bloc), buffer);
}

void write_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer) {
  write_sector(cob(vol,bloc), sob(vol,bloc), buffer);
}

void frmt_bloc(unsigned int vol, unsigned int bloc, unsigned int value) {
  format_sector(cob(vol,bloc),sob(vol,bloc),value);
}

void read_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize) {
  read_sectorn(cob(vol,bloc), sob(vol,bloc), buffer, bufsize);
}

void write_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize) {
  write_sectorn(cob(vol,bloc), sob(vol,bloc), buffer, bufsize);
}

void load_super(unsigned int vol) {
  read_blocn(vol,0,(unsigned char*) &superbloc,sizeof(struct superbloc_s));
  current_vol = vol;
  return;
}

void save_super() {
  write_blocn(current_vol,0,(unsigned char *) &superbloc, sizeof(struct superbloc_s));
}
void init_super(unsigned int vol, char* name) {
  int i;
  load_super(vol);
  if (superbloc.magic == MAGIC_SB) {
    fprintf(stderr, "Superbloc déjà initialisé.\n");
    exit(EXIT_FAILURE);
  }
  else {
    int l;
    superbloc.magic = MAGIC_SB;
    strncpy(superbloc.name, name, MAX_TAILLE);
    superbloc.first_free_bloc = 1;
    superbloc.root = 0;
    superbloc.nb_free = mbr.vol[vol].size-1;
    save_super(vol);
    for (i = 1, l = mbr.vol[vol].size; i < l; i++) {
      struct free_bloc_s fb;
      fb.next = (i+1)%mbr.vol[vol].size;
      fb.magic = MAGIC_FREE;
      write_blocn(vol, i, (unsigned char *) &fb, sizeof(struct free_bloc_s));
    }
    printf("Volume %d initialisé\n", vol);
  }
}
