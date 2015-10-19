#include <stdlib.h>
#include <stdio.h>
#include "include/hardware.h"
#include "hw_config.h"

/*
 * Déplace la tête de lecture sur la piste cylinder et le secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 */
void seek (unsigned int cylinder, unsigned int sector) {
  _out(HDA_DATAREGS, (cylinder >> 8) & 0xFF);
  _out(HDA_DATAREGS + 1, (cylinder) & 0xFF);
  _out(HDA_DATAREGS + 2, (sector >> 8) & 0xFF);
  _out(HDA_DATAREGS + 3, (sector) & 0xFF);
  _out(CMD_REG,CMD_SEEK);
  return;
}
/*
 * Lit les données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer où seront stockées les données lues
 */
void read_sector(unsigned int cylinder,unsigned int sector, unsigned char *buffer) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG,CMD_READ);
  _sleep(HDA_IRQ);
  for (i=0; i<HDA_SECTORSIZE ;i++)
    buffer[i]=MASTERBUFFER[i];
  return;
}
/*
 * Lit au maximum n octets des données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer où seront stockées les données lues
 * @param bufsize la taille des données à lire
 */
void read_sectorn(unsigned int cylinder,unsigned int sector, unsigned char *buffer, unsigned int bufsize) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG,CMD_READ);
  _sleep(HDA_IRQ);
  for (i=0; i<bufsize ;i++)
    buffer[i]=MASTERBUFFER[i];
  return;
}
/*
 * Ecrit des données sur la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer les données à écrire
 */
void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  for (i=0; i<HDA_SECTORSIZE ; i++) {
    MASTERBUFFER[i] = buffer[i];
  }
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG, CMD_WRITE);
  _sleep(HDA_IRQ);
  return;
}
/*
 * Ecrit au maximum n octets sur la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer les données à écrire
 * @param bufsize la taille des données à écrire
 */
void write_sectorn(unsigned int cylinder, unsigned int sector, const unsigned char *buffer, unsigned int bufsize) {
  int i;
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  for (i=0; i<bufsize ; i++) {
    MASTERBUFFER[i] = buffer[i];
  }
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1,1);
  _out(CMD_REG, CMD_WRITE);
  _sleep(HDA_IRQ);
  return;
}
/*
 * Formate les données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param value la valeur à initialiser
 */
void format_sector(unsigned int cylinder,unsigned int sector, unsigned int value) {
  seek(cylinder, sector);
  _sleep(HDA_IRQ);
  _out(HDA_DATAREGS, 0);
  _out(HDA_DATAREGS + 1, 1);
  _out(HDA_DATAREGS + 2, (value >> 24) & 0xFF);
  _out(HDA_DATAREGS + 3, (value >> 16) & 0xFF);
  _out(HDA_DATAREGS + 4, (value >> 8) & 0xFF);
  _out(HDA_DATAREGS + 5, (value) & 0xFF);
  _out(CMD_REG, CMD_FORMAT);
  _sleep(HDA_IRQ);
  return;
}
/*
 * Formate les données sur nbsector secteurs à partir de la piste cylinder, secteur sector mais à l'envers
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param nsector le nombre de secteur à formater
 * @param value la valeur à initialiser
 */
void format_sector_reverse(unsigned int cylinder, unsigned int sector, unsigned int nbsector, unsigned int value){
  int i;
  for (i = 0; i < nbsector; i++){
    format_sector(cylinder, sector-i, value);
  }
  return;
}
