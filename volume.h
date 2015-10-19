/*
 * volume.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef VOLUME_H
#define VOLUME_H

void read_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer);

void write_bloc(unsigned int vol, unsigned int bloc, unsigned char *buffer);

void frmt_bloc(unsigned int vol, unsigned int bloc, unsigned int value);

void read_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);

void write_blocn(unsigned int vol, unsigned int bloc, unsigned char *buffer, unsigned int bufsize);
  
#endif
