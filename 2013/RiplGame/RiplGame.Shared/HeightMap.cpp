#include "pch.h"

#pragma pack(2)
// for whatever reason it's not reading it in :/
typedef struct tagBITMAPFILEHEADER {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;


void HeightMapLoad(char* fname, HeightMapInfo *hm)
{
	//HeightMapInfo *hm = new HeightMapInfo();
	assert(hm != nullptr);

	FILE *filepointer;
	BITMAPFILEHEADER Header;	// info about bitmap file
	BITMAPINFOHEADER Info;		// info about bitmap
	float factor = 2.0f;		// divide bitmap "height" to get more realistic edges

	// first open the file
	errno_t errorCode = fopen_s(&filepointer, fname, "rb"); // read the file as a binary file
	if (filepointer == NULL) {	// file did not open correctly
		fclose(filepointer);
		hm->loaded = false;
	}

	// file has opened successfully

	// firstly, read the bitmap file info header into Header
	fread(&Header, sizeof(BITMAPFILEHEADER), 1, filepointer);

	// second, read the bitmap info header into Info
	fread(&Info, sizeof(BITMAPINFOHEADER), 1, filepointer);

	// get the dimensions of bitmap so we can generate heightmap
	hm->width = Info.biWidth;
	hm->length = Info.biHeight;

	// now calculate the size of bitmap in bytes
	// use 3 as the multiplier (1 each for RGB) even if grayscale image
	int bitmapSize = hm->width * hm->length * 3;

	// create an array to store the bitmap data
	unsigned char* bitmap = new unsigned char[bitmapSize];

	// move pointer to the beginning of the actual image
	// SEEK_SET = beginning of file
	fseek(filepointer, Header.bfOffBits, SEEK_SET);

	// read bitmap file into our bitmap array
	fread(bitmap, 1, bitmapSize, filepointer);

	// close bitmap file
	fclose(filepointer);

	// transfer data into heightmap struct
	hm->heightmap = new XMFLOAT3[hm->width * hm->length];
	assert(hm->heightmap != NULL);

	// keep track of position
	int bitmapIndex = 0;

	// index to calculate location to store height in heightmap array
	int heightIndex;
	unsigned char height;

	// create our heightmap from the bitmap data
	// loop through the length/height of the map
	for (int i = 0; i < hm->length; i++) {
		// loop through the width of map
		for (int k = 0; k < hm->width; k++) {
			height = bitmap[bitmapIndex];

			// index location = width + 2*length
			heightIndex = k + (hm->length * i);

			// store the position and height data in heightmap
			hm->heightmap[heightIndex].x = (float)k;
			hm->heightmap[heightIndex].y = (float)height / factor;
			hm->heightmap[heightIndex].z = (float)i;
			
			// increment to next pixel (skip 2 other RGB components)
			bitmapIndex += 3;
		}
	}

	// cleanup memory
	delete[] bitmap;

	// heightmap successfully generated :D
	hm->loaded = true;

	// return HeightMapInfo pointer
	//return hm;
}
