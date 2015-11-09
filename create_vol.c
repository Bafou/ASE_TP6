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
  int i,l;
  unsigned int cylinder, sector, size;
  enum type_e type;
  struct volume_s volume;

  if (argc != 5) {
    printf("Usage:\n\tcreate_vol <cylinder (0 to 15)> <sector (0 to 15)> <size> <type: ANNEXE|BASE|OTHER>\n");
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

  if (cylinder >= HDA_MAXCYLINDER || cylinder < 0){
    printf("Usage:\n\tcreate_vol <cylinder (0 to 15)> <sector (0 to 15)> <size> <type: ANNEXE|BASE|OTHER>\n");
    exit(EXIT_FAILURE);
  }
  if (sector >= HDA_MAXSECTOR || sector < 0){
    printf("Usage:\n\tcreate_vol <cylinder (0 to 15)> <sector (0 to 15)> <size> <type: ANNEXE|BASE|OTHER>\n");
    exit(EXIT_FAILURE);
  }
  if (size > HDA_SECTORSIZE){
    printf("Taille trop grande!\nUsage:\n\tcreate_vol <cylinder (0 to 15)> <sector (0 to 15)> <size> <type: ANNEXE|BASE|OTHER>\n");
    exit(EXIT_FAILURE);
  }

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  load_mbr();

  for (i = 0,l = mbr.nb_vol; i < l; i++){
    if (mbr.vol[i].cylinder == cylinder){
      if (sector == mbr.vol[i].sector){
        printf("Un volume débutant à ce secteur existe déjà.\n");
        exit(0);
      }
      else if (sector < mbr.vol[i].sector){
        if (size > (mbr.vol[i].sector - sector)){
          printf("Le volume que vous voulez créer est trop grand.\n");
          exit(0);
        }
      }
      else {
        if (mbr.vol[i].size > (sector - mbr.vol[i].sector)){
          printf("Un volume occupe déjà l'espace que vous voulez.\n");
          exit(0);
        }
      }
    }
  }
  volume.cylinder = cylinder;
  volume.sector = sector;
  volume.size = size;
  volume.type = type;

  mbr.vol[mbr.nb_vol] = volume;
  mbr.nb_vol = mbr.nb_vol+1;
  save_mbr();
  printf("Volume (%d, %d) de taille %d et de type %s créé.\n", cylinder, sector, size, argv[4]);

  return 0;
}
