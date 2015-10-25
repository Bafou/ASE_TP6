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
  unsigned int vol, bloc;

  if (argc != 2) {
    printf("Usage:\n\tremove_vol <vol (0 to 7>\n");
    exit(EXIT_FAILURE);
  }

  // Récupération des arguments
  vol = atoi(argv[1]);
  if (vol >= MAX_VOL || vol < 0) {
    fprintf(stderr, "%d ne correspond pas à un numéro de volume\n", vol);
    exit(EXIT_FAILURE);
  }

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;


  // Création du volume
  load_mbr();
  if (vol >= mbr.nb_vol) {
    fprintf(stderr, "Le volume %d n'est pas présent\n", vol);
    exit(EXIT_FAILURE);
  }
  for (i = vol; i < mbr.nb_vol-1;i++){
    mbr.vol[i] = mbr.vol[i+1];
  }
  mbr.nb_vol = mbr.nb_vol-1;
  save_mbr();

  return 0;
}
