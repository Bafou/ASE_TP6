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

int create_volume(unsigned int cylinder, unsigned int sector, int size, enum vol_type_e type){
  int i,l;
  unsigned int cyl_target, sec_target;
  struct volume_s vol;
  char name[MAX_TAILLE];
  load_mbr();
  if (mbr.nb_vol >= MAX_VOL){
    fprintf(stderr,"Error: Reached maximum number of volume.\n");
    return -1;
  }
  if (cylinder >= HDA_MAXCYLINDER){
    fprintf(stderr,"Error: Specified cylinder is too high.\n");
    return -1;
  }
  if (sector >= HDA_MAXSECTOR){
    fprintf(stderr,"Error: Specified sector is too high.\n");
    return -1;
  }
  cyl_target = cylinder + ((sector+size) / HDA_MAXSECTOR);
  sec_target = ((sector+size) % HDA_MAXSECTOR) -1;
  if (cyl_target >= HDA_MAXCYLINDER){
    fprintf(stderr,"Error: Specified size is too high.\n");
    return -1;
  }
  for (i = 0,l = mbr.nb_vol; i < l; i++) {
    unsigned int cyl_target_i, sec_target_i;
    cyl_target_i = mbr.vol[i].cylinder + ((mbr.vol[i].sector+mbr.vol[i].size)/ HDA_MAXSECTOR);
    sec_target_i = ((mbr.vol[i].sector+mbr.vol[i].size) % HDA_MAXSECTOR) - 1;
    if (!((cylinder == mbr.vol[i].cylinder && size <= (mbr.vol[i].sector - sector))
      || (cyl_target < mbr.vol[i].cylinder)
      || (cylinder > cyl_target_i)
      || (cyl_target == mbr.vol[i].cylinder && sec_target < mbr.vol[i].sector)
      || (cylinder == cyl_target_i && sector > sec_target_i)))
    {
      fprintf(stderr, "Error: specified cylinder and sector values will overlap existing volume\n");
      return -1;
    }
  }
  vol.cylinder = cylinder;
  vol.sector = sector;
  vol.size = size;
  vol.type = type;
  mbr.vol[mbr.nb_vol] = vol;
  mbr.nb_vol++;
  save_mbr();
  sprintf(name, "Volume %d [%d,%d,%d]", (mbr.nb_vol-1),cylinder,sector,size);
  if (init_super(mbr.nb_vol-1,name)){
    printf("%s initialisé\n", name);
  }
  printf("Volume %d de taille %d créé (début [%d,%d], fin [%d,%d]).\n",(mbr.nb_vol-1),size,cylinder,sector,cyl_target,sec_target);
  return (mbr.nb_vol-1);
}

int main(int argc, char** argv) {
  int i,l;
  unsigned int cylinder, sector, size;
  enum vol_type_e type;
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
  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;

  l = create_volume(cylinder, sector, size, type);
  if (l < 0) {
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
