/*
 * Driver.h
 * Authors : Honoré NINTUNZE & Antoine PETIT
 */
#ifndef DRIVER_H
#define DRIVER_H

/*
 * Lit les données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer où seront stockées les données lues
 */
void read_sector(unsigned int cylinder, unsigned int sector, unsigned char * buffer);
/*
 * Lit au maximum n octets des données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer où seront stockées les données lues
 * @param bufsize la taille des données à lire
 */
void read_sectorn(unsigned int cylinder, unsigned int sector, unsigned char * buffer, unsigned int bufsize);
/*
 * Ecrit des données sur la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer les données à écrire
 */
void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char * buffer);
/*
 * Ecrit au maximum n octets sur la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param buffer les données à écrire
 * @param bufsize la taille des données à écrire
 */
void write_sectorn(unsigned int cylinder, unsigned int sector, const unsigned char * buffer, unsigned int bufsize);
/*
 * Formate les données de la piste cylinder, secteur sector
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param value la valeur à initialiser
 */
void format_sector(unsigned int cylinder,unsigned int sector, unsigned int value);
/*
 * Formate les données sur nbsector secteurs à partir de la piste cylinder, secteur sector mais à l'envers
 * @param cylinder le numéro de piste
 * @param sector le numéro de secteur
 * @param nsector le nombre de secteur à formater
 * @param value la valeur à initialiser
 */
void format_sector_reverse(unsigned int cylinder, unsigned int sector, unsigned int nbsector, unsigned int value);
#endif
