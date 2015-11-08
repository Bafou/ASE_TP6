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
    fprintf(stderr, "Aucun bloc libre.\n");
    return -1;
  }
  res = superbloc.first_free_bloc;
  struct free_bloc_s fb;
  read_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struc free_bloc_s));
  superbloc.first_free_bloc = fb.next;
  superbloc.nb_free--;
  save_super(current_vol);
  return res;
}

int free_bloc() {
  int i,l;
    if (superbloc.nb_free == mbr.vol[vol].size-1) {
      fprintf(stderr, "Tous les blocs disponibles sont libres.\n");
      return -1;
    }
    else {
      for (i = 1,l = mbr[vol].size; i < l; i++) {
        struct free_bloc_s fb;
        fb.next = (i+1)%mbr[vol].size;
        read_blocn(current_vol, i, (unsigned char *) &fb, sizeof(struct free_bloc_s));
        if (!(fb.magic == MAGIC_FREE)) {
          unsigned int res_free = superbloc.first_free_bloc;
          superbloc.first_free_bloc = i;
          superbloc.nb_free++;
          fb.next = res_free;
          fb.magic = MAGIC_FREE;
          save_super(current_vol);
          write_blocn(current_vol, i, (unsigned char *) &fb, sizeof(struct free_bloc_s));
          return i;
        }
      }
      return -2;
    }
}
