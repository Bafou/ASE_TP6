#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Driver.h"
#include "mbr.h"


struct mbr_s mbr;

int load_mbr() {
  read_sectorn(0, 0, (unsigned char *) &mbr, sizeof(struct mbr_s));
  if (mbr.magic != MAGIC) {
    memset(&mbr,0,sizeof(struct mbr_s));
    mbr.magic = MAGIC;
    mbr.nb_vol = 0;
    return 0;
  }
  else {
    return 1;
  }
}

void save_mbr() {
  write_sectorn(0, 0, (unsigned char *) & mbr, sizeof(struct mbr_s));
}
