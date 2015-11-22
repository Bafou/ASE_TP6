#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "volume.h"
#include "mbr.h"
#include "include/hardware.h"
#include "hw_config.h"

static void
load_current_volume ()
{
    char* current_volume_str;

    current_volume_str = getenv("CURRENT_VOLUME");
    if (current_volume_str == NULL) {
      fprintf(stderr, "Current volume can't be set. No environment variable.\n");
      exit (EXIT_FAILURE);
    }
    current_vol = strtol(current_volume_str, NULL, 10);
    load_super(current_vol);
}

static void
emptyIT()
{
    return;
}

/* ------------------------------
   Initialization and finalization fucntions
   ------------------------------------------------------------*/
void
mount()
{
    int status, i;

    /* Hardware initialization */
    assert(init_hardware(HARDWARE_INI));

    /* Interrupt handlers */
    for(i=0; i<16; i++)
	   IRQVECTOR[i] = emptyIT;

    /* Allows all IT */
    _mask(1);

    /* Load MBR and current volume */
    load_mbr();
    load_current_volume();
}

void
umount()
{
    /* save current volume super bloc */
    save_super();

    /* bye */
}
