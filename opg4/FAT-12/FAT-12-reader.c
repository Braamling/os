/* In this file a simple simulated FAT12 type file system is defined.
	 The relevant structures in this file system will be:
	 1) The disk information block BPB, describing the properties of the
			other structures.
	 2) The FATs
	 3) The root directory. This directory must be present.
	 4) Data blocks
	 All structures will be allocated in units of 512 bytes, corresponding
	 to disk sectors.
	 Data blocks are allocated in clusters of spc sectors. The FATs refer
	 to clusters, not to sectors.
	 The BPB below was derived from the information provided on the
	 Atari TOS floppy format; it is also valid for MS DOS floppies,
	 but Atari TOS allows more freedom in choosing various values.

	 A start has been made to incorporate VFAT long file names (LFN),
	 but this was not completed.
	 Error checking is barely present - incorporating this is part
	 of an OS assignment.

	 G.D. van Albada
	 (c) IvI, Universiteit van Amsterdam, 2012
	 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

/* The following structure describes the boot block - the first 512 bytes on
	 the floppy disk
	 Actually, the following definition does not guarantee 100% that all items
	 are mapped correctly, as the compiler may insert padding fields.
	 However, it is highly unusual for compilers to insert such padding
	 between variables of the same length (8 bit items will be alligned
	 on 8 bit boundaries).
	 The definition is robust for big endian/little endian differences
	 through the use of the "toShort" and "toLong" macros
*/
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

/* Two macros to help convert bytes to short values and 
	 to 4 byte integers
*/
#define toShort(b) ((b[0] & 0xff) + 256 * (b[1] & 0xff))
#define toLong(b) ((b[0] & 0xff) + (((long)(b[1] & 0xff)) << 8) +\
									(((long)(b[2] & 0xff)) << 16) +\
									(((long)(b[3] & 0xff)) << 24))

/* print a directory entry
*/
void printDirEntry(dirEntry * e) {
		int i;
		if (e->attrib == 0x0f)
		{
				printf("LFN:");
				for (i = 1; i < 32; i++)
				{
						if (e->name[i] >= ' ') printf("%c", e->name[i]);
						else if (e->name[i]) printf("&%x;", e->name[i]);
				}
				printf("\n");
		} else
		{
				for (i = 0; i < 8; i++)
				{
						if (e->name[i] < ' ')
						{
								break;
						}
						printf("%c", e->name[i]);
				}
				printf(".");
				for (i = 0; i < 3; i++)
				{
						if (e->ext[i] < ' ')
						{
								break;
						}
						printf("%c", e->ext[i]);
				}
				printf(" (%x)", e->attrib);
				for (i = 0; i < 10; i++)
				{
						printf("%2x", e->zero[i]);
				}
				printf(" time: %hu date: %hu start: %hu length: %ld\n",
						toShort(e->time),
						toShort(e->date), toShort(e->start), toLong(e->length));
		}
}

/* Get the name of a file with extention */
int get_file_name(dirEntry * e, char *name_holder) {
		int i, j;
		j = 0;

		for(i = 0; i < 8; i++){
			if(e->name[i] != ' '){
				name_holder[j] = e->name[i];
				j++;
			}
		}

		name_holder[j] = '.';
		j++;

		for(i = 0; i < 3; i++){
			if(e->ext[i] != ' '){
				name_holder[j] = e->ext[i];
				j++;
			}
		}

		name_holder[j] = '\0';

}

/* Get the name of a folder without an extention */
int get_folder_name(dirEntry * e, char *name_holder) {
		int i, j;
		j = 0;

		for(i = 0; i < 8; i++){
			if(e->name[i] != ' '){
				name_holder[j] = e->name[i];
				j++;
			}
		}

		name_holder[j] = '\0';

}

/* The following code should obtain the total actual number of clusters
	 in a chain starting at some directory-entry.start
*/
int followDirEntry(dirEntry *e, unsigned short * sFAT) {
		int cur = toShort(e->start);
		int next;
		int nclusters = 0;
		int size = toLong(e->length);
		int nexpected = 0;
		nexpected = (size + clusterSize - 1) / clusterSize;
		printf("Following chain from %d\n", cur);
		/* For directories a length of zero is generally specified.
			 This code will thus read only a single cluster for directories. */
		if (e->attrib == 0x0f)
		{
				return 0;
		}
		if (e->attrib & 0x08)
		{
				printf("Volume label, not a file\n");
				return 0;
		}
		if (cur < 2)
		{
				printf("Not a valid starting cluster\n");
				return 0;
		}
		do
		{
				nclusters++;
				next = sFAT[cur];
				printf("%d ", next);
				cur = next;

		} while (next && (next < 0x0FF0) && (nclusters < nexpected));
		printf("\nNclusters = %d\n", nclusters);
		return nclusters;
}

/* This routine will read a file from disk and store it in a buffer
*/
int 
bufferFile(dirEntry *e, unsigned short * sFAT, char ** buffer) {
/* First find number of clusters in file */
		int cur = toShort(e->start);
		int nclusters = followDirEntry(e, sFAT);
		int nbytes;
		int nread = 0;
		int next;
		int offset = 0;
		(*buffer) = NULL;
		if (nclusters == 0) return 0;

		*buffer = calloc(nclusters * bps * spc, 1);
		do
		{
				lseek(fid, (cur + dataStart) * clusterSize, SEEK_SET);
				if (clusterSize !=
								(nbytes = read(fid, (*buffer) + offset, clusterSize)))
				{
						printf("Disk read error, expected %d bytes, read %d\n",
									 clusterSize, nbytes);
						printf("Attempting to read cluster %d\n", cur);
						return -1;
				}
				nread++;
				offset += clusterSize;
				next = sFAT[cur];
				cur = next;
		} while (next && (next < 0x0FF0) && (nread < nclusters));
		if (next < 0x0FF0)
		{
		/* not a normal end of chain */
				printf("Broken file, read %d clusters, expected %d,"
							 "next cluster would be at %d\n",
								nread, nclusters, next);
				return -2;
		}
		return nclusters;
}
		
/* Read the entries in a directory (recursively).
	 Files are read in, allowing further processing if desired
*/
int 
readDirectory(dirEntry *dirs, int Nentries, unsigned short * sFAT)
{
	FILE *pFile;
	int i, j;
	size_t buffersize;
	char * buffer = NULL, * filename[12], * foldername[8];
	int nclusters = 0;
	for (j = i = 0; i < Nentries; i = j + 1)
	{
			printDirEntry(dirs + i);
			for (j = i; j < Nentries; j++)
			{
					if (dirs[j + 1].attrib != 0x0f) break;
					printDirEntry(dirs + j);
			}
			if ((dirs[i].name[0] == 0x05) || (dirs[i].name[0] == 0xe5))
			{
					printf("Deleted entry\n");
			} else if (dirs[i].name[0] > ' ' && (dirs[i].name[0] != '.'))
			{
					free(buffer);
					nclusters = bufferFile(dirs + i, sFAT, &buffer);
					if (buffer && (dirs[i].attrib & 0x10) && (nclusters > 0))
					{
							int N;
							/* Creating the new found directory and entering it. */
							get_folder_name(dirs + i, foldername);
							mkdir(foldername, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
							chdir(foldername);


							N = nclusters * clusterSize / sizeof(dirEntry);
							printf("Reading directory\n");
							readDirectory((dirEntry *) buffer, N, sFAT);

					}else{
						get_file_name(dirs + i, filename);

						/* Create a file when not existing */
						pFile = fopen(filename, "ab+");

						/* buffersize should replace (nclusers * clusterSize) in the future */
						buffersize = toLong(dirs->length);

						fwrite(buffer , 1 , nclusters * clusterSize, pFile);
						fclose(pFile);
					}
					
					
			}
			
	}
	chdir("..");
	free(buffer);
	return 0;
}

/* Convert a 12 bit FAT to 16bit short integers
*/
void
expandFAT(uint8_t * FAT, unsigned short * sFAT, int entries)
{
		int i;
		int j;
		for (i = 0, j = 0; i < entries; i += 2, j += 3)
		{
				sFAT[i] = FAT[j] + 256 * (FAT[j + 1] & 0xf);
				sFAT[i + 1] = ((FAT[j + 1] >> 4) & 0xf) + 16 * FAT[j + 2];
		}
}

/* Convert a FAT represented as 16 bit shorts back to 12 bits to
	 allow rewriting the FAT
*/
void
shrinkFAT(uint8_t * FAT, unsigned short * sFAT, int entries)
{
		int i;
		int j;
		for (i = 0, j = 0; i < entries; i += 2, j += 3)
		{
				FAT[j] = sFAT[i] & 0xff;
				FAT[j + 1] = ((sFAT[i] & 0x0f00) >> 8) + ((sFAT[i + 1] & 0x000f) << 4);
				FAT[j + 2] = (sFAT[i + 1] & 0x0ff0) >> 4;
		}
}

/* We'll allow for at most two FATs on a floppy, both as 12 bit values and
	 also converted to 16 bit values
*/
uint8_t * FAT1;
uint8_t * FAT2;

unsigned short * sFAT1;
unsigned short * sFAT2;

int main(int argc, char * argv[]) {
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

		mkdir("disk", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		chdir("disk");

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
