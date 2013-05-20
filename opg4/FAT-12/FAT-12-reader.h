typedef struct BPB
{
   uint8_t BRA[2];       /* Branch to boot code           */
   uint8_t filler[6];    /* reserved for OEM              */
   uint8_t  vsn[3];      /* volume-serial number 24 bit   */
   uint8_t  bps[2];      /* bytes per sector              */
   uint8_t  spc;         /* sectors per cluster           */
   uint8_t  res[2];      /* reserved                      */
   uint8_t  NFats;       /* number of FATs                */
   uint8_t  Ndirs[2];    /* number of directory entries   */
   uint8_t  Nsects[2];   /* number of sectors on media    */
   uint8_t  Media;       /* Media descriptor              */
   uint8_t  spf[2];      /* Sectors per FAT               */
   uint8_t  spt[2];      /* Sectors per track             */
   uint8_t  Nsides[2];   /* Sides on media                */
   uint8_t  Nhid[2];     /* hidden sectors                */
   uint8_t  boot[482];   /* boot code                     */
} BPB;

/* The following structure describes a directory entry.
   The definition is valid on both big and little-endian systems,
   but length, date, start and time must be computed using
   toShort and toLong
*/
typedef struct DIRENTRY
{
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attrib;
    uint8_t zero[10];	/* Actually not zero, used for various purposes
                           in various types of FAT 12 systems */
    uint8_t time[2];
    uint8_t date[2];
    uint8_t start[2];
    uint8_t length[4];
} dirEntry;

void printDirEntry(dirEntry * e);

int followDirEntry(dirEntry *e, unsigned short * sFAT);

int bufferFile(dirEntry *e, unsigned short * sFAT, char ** buffer);

int readDirectory(dirEntry *dirs, int Nentries, unsigned short * sFAT);

void expandFAT(uint8_t * FAT, unsigned short * sFAT, int entries);

void shrinkFAT(uint8_t * FAT, unsigned short * sFAT, int entries);

