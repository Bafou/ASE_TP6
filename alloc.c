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
    return -1;
  }
  res = superbloc.first_free_bloc;
  struct free_bloc_s fb;
  read_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struct free_bloc_s));
  superbloc.first_free_bloc = fb.next;
  fb.magic = !MAGIC_FREE;
  write_blocn(current_vol, res, (unsigned char *) &fb, sizeof(struct free_bloc_s));
  superbloc.nb_free--;
  save_super();
  
  return res;
}

void free_bloc(unsigned int bloc) {
  struct free_bloc_s fb;
  if ((bloc >=0) && (bloc < mbr.vol[current_vol].size)) {
    read_blocn(current_vol, bloc, (unsigned char *) &fb, sizeof(struct free_bloc_s));
  	if (fb.magic != MAGIC_FREE) {
  	  unsigned int res_free = superbloc.first_free_bloc;
      fb.next = superbloc.first_free_bloc;
      superbloc.first_free_bloc = bloc;
  	  superbloc.nb_free++;
  	  fb.magic = MAGIC_FREE;
  	  save_super();
  	  write_blocn(current_vol, bloc, (unsigned char *) &fb, sizeof(struct free_bloc_s));
  	  return;
	  }
	    printf("Le bloc est déjà libéré\n");
	  return;
	}
	fprintf(stderr, "Vous essayez de libérer un bloc en dehors du volume\n");
	return;
}
