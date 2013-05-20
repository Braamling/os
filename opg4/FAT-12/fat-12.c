#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

#include "FAT-12-reader.h"

uint8_t * FAT1;
uint8_t * FAT2;

/* The following are default values. Most are recomputed on basis of the 
   information in the bootblock.
*/
static int clusterSize = 1024;
static int bps = 512;
static int spc = 2;
static int dataStart = 0;

/* The file identified for the input file is stored here,
   as a global identifier
*/
static int fid;

unsigned short * sFAT1;
unsigned short * sFAT2;

int
main(int argc, char * argv[])
{
    BPB bootsector;
    int rv = 0;
    int entries;
    int NFATbytes;
    int dataSectors;
    int clusters;
    int i;
    int Ndirs;
    int NdirSectors;
    int nread;
    dirEntry *dirs;

    printf("size of bootsector = %u\n", sizeof(BPB));
    if (argc > 1)
    {
        fid = open(argv[1], O_RDONLY);
    } else
    {
        printf("Need one file argument\n");
        exit(-1);
    }
    if ((sizeof(BPB) != (rv = read(fid, &bootsector, sizeof(BPB)))))
    {
        printf("Read error %d\n", rv);
        exit(-2);
    }
    printf("Serial nr. %u-%u-%u\n", bootsector.vsn[0],
             bootsector.vsn[1], bootsector.vsn[2]);
    printf("bps = %hu\n", bps = toShort(bootsector.bps));
    printf("spc = %hu\n", spc = bootsector.spc);
    printf("res = %hu\n", toShort(bootsector.res));
    printf("NFats = %hu\n", bootsector.NFats);
    printf("Ndirs = %hu\n", Ndirs = toShort(bootsector.Ndirs));
    printf("Nsects = %hu\n", toShort(bootsector.Nsects));
    printf("Media = %hu\n", bootsector.Media);
    printf("spf = %hu\n", toShort(bootsector.spf));
    printf("spt = %hu\n", toShort(bootsector.spt));
    printf("Nsides = %hu\n", toShort(bootsector.Nsides));
    printf("Nhid = %hu\n", toShort(bootsector.Nhid));
    if (!bps || !bootsector.NFats  || !toShort(bootsector.Nsects))
    {
        printf("This is not a FAT-12-type floppy format\n");
        exit(-2);
    }
    /* Compute the number of entries in the FAT
       The number of bytes is BPS * spf
       The maximum number of entries is 2/3 of that.
       On the other hand, we have a maximum of x data-sectors,
       where x = Nsects - 1 - NFats * spf
       or mayby x = Nsects - res
    */
    NFATbytes = toShort(bootsector.bps) * toShort(bootsector.spf);
    entries = (2 * NFATbytes) / 3;
    dataSectors = toShort(bootsector.Nsects) - 1 /* boot */ -
                  bootsector.NFats * toShort(bootsector.spf);
    clusters = dataSectors / bootsector.spc;
    clusterSize = spc * bps;
    printf("entries = %u, dataSectors = %u, clusters = %u\n", entries,
           dataSectors, clusters);
    FAT1 = malloc(NFATbytes);
    FAT2 = malloc(NFATbytes);
    nread = read(fid, FAT1, NFATbytes);
    if (nread != NFATbytes)
    {
        printf("Unexpected EOF\n");
    }
    for (i = 1; i < bootsector.NFats; i++)
    {
        nread = read(fid, FAT2, NFATbytes);
        if (nread != NFATbytes)
        {
            printf("Unexpected EOF\n");
        }
    }
    sFAT1 = calloc(entries + 1, sizeof(unsigned short));
    sFAT2 = calloc(entries + 1, sizeof(unsigned short));
    expandFAT(FAT1, sFAT1, entries);
    expandFAT(FAT2, sFAT2, entries);
    printf("FAT1: %hu  %hu  %hu  %hu  %hu  %hu\n", sFAT1[0],
            sFAT1[1], sFAT1[2], sFAT1[3], sFAT1[4], sFAT1[5]);
    printf("FAT2: %hu  %hu  %hu  %hu  %hu  %hu\n", sFAT2[0],
            sFAT2[1], sFAT2[2], sFAT2[3], sFAT2[4], sFAT2[5]);
    
    i = bps / sizeof(dirEntry);
    NdirSectors = (Ndirs + i - 1) / i;
    dataStart = 1 + (bootsector.NFats * NFATbytes) / bps + NdirSectors - 2;
    dirs = calloc(Ndirs, sizeof(dirEntry));
    printf("dataStart = %d\n", dataStart);
    nread = read(fid, dirs, bps * NdirSectors);
    if (nread != bps * NdirSectors)
    {
        printf("Unexpected EOF\n");
    }
    readDirectory(dirs, Ndirs, sFAT1);
    
    
    return 0;
}
