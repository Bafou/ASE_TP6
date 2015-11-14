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
	int i,volume,bloc,res;

  if (argc != 3){
    printf("Usage:\n\tnew_bloc <volume> <bloc>\n");
    exit(0);
  }
  volume = atoi(argv[1]);
  bloc = atoi(argv[2]);
  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;
  load_mbr();
  if (volume < 0 || volume >= mbr.nb_vol){
    printf("Le volume %d n'existe pas.\n",volume);
    exit(0);
  }
  if (bloc < 0 || bloc >= mbr.vol[volume].size){
    printf("Le bloc %d n'est pas présent dans le volume %d\n",bloc,volume);
    exit(0);
  }
  load_super(volume);
  free_bloc(bloc);
  return 0;
}
