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
  int i;
  // Initialisation
  assert(init_hardware(HARDWARE_INI));
  for(i = 0; i < 15; i++)
    IRQVECTOR[i] = empty_it;
  load_mbr();
  system("./print_vol");
  printf("\nCreation du volume 2 3 de taille 4 et de type ANNEXE\n");
  system("./create_vol 2 3 4 ANNEXE");
  
  system("./print_vol");
  init_super(0,"Volume 0");
  load_super(0);
  system("./print_vol");
  new_bloc();
  new_bloc();
  new_bloc();
  new_bloc();// affiche le message comme quoi il n'y a plus de bloc libre
  free_bloc(2);
  free_bloc(2); // affiche le message comme quoi le bloc a déjà été libéré
  free_bloc(5); //affiche le message comme quoi le bloc qu'on essaie de libéré n'est pas bon
  new_bloc(); // doit fonctionner
  free_bloc(2);
  // test sur free : free n'importe quel bloc, retenter de le free, tenter de free un bloc en dehors du volume
}
