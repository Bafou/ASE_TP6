#include <stdio.h>
#include <stdlib.h>
#include "hw_config.h"
#include "Driver.h"
#include "include/hardware.h"
#include <assert.h>

#define BUFSIZE 256

static void empty_it();

int
main (int argc, char ** argv) {
  int i;
  unsigned int cylinder, sector, nbsector, value;
  unsigned char buffer[BUFSIZE];

  if (argc != 4) {
    printf("Usage:\n\tfrmt <cylinder (1 to 16)> <sector (1 to 16)> <value>\n");
    exit(EXIT_FAILURE);
  }

  // Récupération des arguments
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);
  value = atoi(argv[3]);

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  // Formatage puis affichage
  format_sector(cylinder,sector,value);
  exit(EXIT_SUCCESS);
}

static void
empty_it()
{
    return;
}
