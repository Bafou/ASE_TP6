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
  else {
    return "OTHER";
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
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  if (load_mbr()) {
    printf("Chargement normal du MBR\n");
  }
  else {
    printf("Initialisation d'un MBR\n");
  }

  printf("Il y a %d partions\n",mbr.nb_vol);
  for (i = 0; i < mbr.nb_vol;i++) {
    printf("Partition %d de type %s :\n\tDébute au cylindre %d et au secteur %d, sa taille est de %d blocs\n", i, get_type(mbr.vol[i].type), mbr.vol[i].cylinder, mbr.vol[i].sector, mbr.vol[i].size);
  }
  return 0;
}
