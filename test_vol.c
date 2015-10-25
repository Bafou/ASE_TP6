#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Driver.h"
#include "mbr.h"
#include "volume.h"
#include "include/hardware.h"
#include "hw_config.h"

int main(int argc, char** argv) {

  // Création du volume
  system("./print_vol");
  printf("\nCreation du volume 2 3 de taille 4 et de type ANNEXE\n");
  system("./create_vol 2 3 4 ANNEXE");
  system("./print_vol");
  printf("\nCreation du volume 1 3 de taille 2 et de type BASE\n");
  system("./create_vol 1 3 2 BASE");
  system("./print_vol");
  printf("\nSuppresion du volume 0\n");
  system("./remove_vol 0");
  system("./print_vol");
  return 0;
}
