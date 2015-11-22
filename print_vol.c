#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Driver.h"
#include "mbr.h"
#include "volume.h"
#include "include/hardware.h"
#include "hw_config.h"

char* get_type(int type) {
  if (type == BASE) {
    return "BASE";
  }
  else if (type == ANNEXE) {
    return "ANNEXE";
  }
  else if (type == OTHER) {
    return "OTHER";
  }
  else {
    fprintf(stderr, "Type inconnu (ANNEXE|BASE|OTHER)\n");
    assert(0);
  }
}

static void
empty_it()
{
    return;
}

int main() {
  int i;

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 16; i++)
    IRQVECTOR[i] = empty_it;

  if (load_mbr() == 1) {
    printf("Chargement normal du MBR\n");
  }
  else {
    printf("Initialisation du MBR\n");
  }

  printf("Il y a %d partions\n",mbr.nb_vol);
  for (i = 0; i < mbr.nb_vol;i++) {
    unsigned int cyl_target, sec_target;
    cyl_target = mbr.vol[i].cylinder + ((mbr.vol[i].sector+mbr.vol[i].size)/ HDA_MAXSECTOR);
    sec_target = ((mbr.vol[i].sector+mbr.vol[i].size) % HDA_MAXSECTOR) - 1;
    load_super(i);
    printf("Partition (%s) %d de type %s :\n\tDébut [cylindre %d,secteur %d], fin [cylindre %d,secteur %d], taille: %d blocs, nombre de blocs libres: %d.\n",
      superbloc.name, i, get_type(mbr.vol[i].type), mbr.vol[i].cylinder, mbr.vol[i].sector, cyl_target, sec_target, mbr.vol[i].size, superbloc.nb_free);
  }
  return 0;
}
