#include <stdio.h>
#include <stdlib.h>
#include "hw_config.h"
#include "Driver.h"
#include "include/hardware.h"
#include <assert.h>

#define BUFSIZE 256

static void empty_it();

void
hex_dump(unsigned char * buffer) {
  int i,j;
  for (i = 0; i < 16; i++) {
    printf("%03d :",i*16);
    for (j = 0; j < 16; j++) {
      printf(" %02x",buffer[i*16+j]);
    }
    printf("\n");
  }
  return;
}

int
main (int argc, char ** argv) {
  int i;
  unsigned int cylinder, sector, nbsector, value;
  unsigned char buffer[BUFSIZE];

  if (argc != 5) {
    printf("Usage:\n\tfrmt <cylinder (1 to 16)> <sector (1 to 16)> <nb sectors> <value>\n");
    exit(EXIT_FAILURE);
  }

  // Récupération des arguments
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);
  nbsector = atoi(argv[3]);
  value = atoi(argv[4]);

  printf("nbsector : %04x, value : %08x\n",nbsector,value);
  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  // Formatage puis affichage
  format_sector_reverse(cylinder,sector,nbsector,value);
  for(i = 0; i < nbsector; i++){
    printf("Cylinder %d, sector %d\n", cylinder,sector-i);
    read_sector(cylinder,sector-i,buffer);
    hex_dump(buffer);
    printf("\n");
  }
  exit(EXIT_SUCCESS);
}

static void
empty_it()
{
    return;
}
