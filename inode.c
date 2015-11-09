/*
 * inode.c
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "Driver.h"
#include "volume.h"
#include "mbr.h"
#include "hw_config.h"

void real_inode (unsigned int inumber, struct inode_s *inode);

void write_inode (unsigned int inumber, struct inode_s *inode);

unsigned int create_inode (enum file_type_e type);

int delete_inode (unsigned int inumber);