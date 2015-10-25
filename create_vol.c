#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Driver.h"
#include "mbr.h"
#include "volume.h"
#include "include/hardware.h"
#include "hw_config.h"

static void
empty_it()
{
    return;
}

int main(int argc, char** argv) {
  int i;
  unsigned int cylinder, sector, size;
  enum type_e type;
  struct volume_s volume;

  if (argc != 5) {
    printf("Usage:\n\tcreate_vol <cylinder (1 to 16)> <sector (1 to 16)> <size> <type: ANNEXE|BASE|OTHER>\n");
    exit(EXIT_FAILURE);
  }

  // Récupération des arguments
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);
  size = atoi(argv[3]);
  if (!strcmp(argv[4],"BASE")) {
    type = BASE;
  }
  else if (!strcmp(argv[4],"ANNEXE")) {
    type = ANNEXE;
  }
  else if (!strcmp(argv[4],"OTHER")) {
    type = OTHER;
  }
  else {
    fprintf(stderr, "Type inconnu (ANNEXE|BASE|OTHER)\n");
    exit(EXIT_FAILURE);
  }

  // Initialisation du nouveau volume
  volume.cylinder = cylinder;
  volume.sector = sector;
  volume.size = size;
  volume.type = type;

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

    // Création du volume
  load_mbr();
  mbr.vol[mbr.nb_vol] = volume;
  mbr.nb_vol = mbr.nb_vol+1;
  save_mbr();

  return 0;
}
