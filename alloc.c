/*
 * alloc.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "Driver.h"
#include "volume.h"
#include "mbr.h"
#include "hw_config.h"

int new_bloc() {
  int res;
  if (superbloc.nb_free == 0){
    fprintf(stderr, "Aucun bloc libre\n");
    return -1;
  }
  res = superbloc.first_free_bloc;
  struct free_bloc_s fb;
  read_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struc free_bloc_s));
  superbloc.first_free_bloc = fb.next;
  superbloc.nb_free--;
  save_super();
  return res;
}

void free_bloc() {
  
}
