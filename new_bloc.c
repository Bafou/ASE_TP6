#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Driver.h"
#include "mbr.h"
#include "volume.h"
#include "alloc.h"
#include "include/hardware.h"
#include "hw_config.h"

static void
empty_it()
{
    return;
}

int main(int argc, char** argv) {
	int i,volume,res;

  if (argc != 2){
    fprintf(stderr,"Usage:\n\tnew_bloc <volume>\n");
    exit(0);
  }
  volume = atoi(argv[1]);
  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;
  load_mbr();
  if (volume < 0 || volume >= mbr.nb_vol){
    fprintf(stderr,"Le volume %d n'existe pas.\n",volume);
    exit(0);
  }
  load_super(volume);
  res = new_bloc();
  if (res < 0){
    fprintf(stderr,"Aucun bloc libre.\n");
    exit(0);
  }
  else{
    printf("Volume %d, bloc %d alloué\n",volume, res );
  }
  return 0;
}