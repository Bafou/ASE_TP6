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

int main (int argc, char ** argv) {
  int i;
  unsigned int cylinder, sector;
  unsigned char buffer[BUFSIZE];

  if (argc != 3) {
    printf("Usage:\n\tdmps <cylinder (1 to 16)> <sector (1 to 16)>\n");
    exit(EXIT_FAILURE);
  }

  // Récupération des arguments
  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);

  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  // Lecture du secteur puis affichage
  read_sector(cylinder,sector,buffer);
  hex_dump(buffer);

  exit(EXIT_SUCCESS);
}

static void
empty_it()
{
    return;
}
