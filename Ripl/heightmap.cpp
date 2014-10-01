// http://www.braynzarsoft.net/index.php?p=DX10HeightMap

// Stores the height, width, and array of vertices
struct HeightMapInfo {
  int width; // width of heightmap
  int length; // length of heightmap
  XMFLOAT3 *heightmap; // stores (x, y, z) positions where y is bitmap height
};

// load the bitmap into our heightmap structure
// returns true if succesful, false otherwise
bool LoadHeightmapBitmap(char* fname, HeightMapInfo &hm){
  FILE *filepointer;
  BITMAPINFOHEADER bmInfoHd; // info about bitmap
  BITMAPFILEHEADER bmFileHd; // info about the bitmap file
  float factor = 2.0f; // divide bitmap "height" to get more realistic edges

  // first we need to open the file
  filepointer = fopen(fname, 'rb') // read the file as a binary file
  if (filepointer == NULL) { // file did not open correctly
    return false;
  }

  // file has opened succesfully

  // Firstly, read the bitmap file info header into bmFileHd
  fread(&bmFileHd, sizeof(BITMAPFILEHEADER), 1, filepointer)

  // Second, read the bitmap info header into bmInfoHd
  fread(&bmInfoHd, sizeof(BITMAPINFOHEADER), 1, filepointer)

  // Get dimensions of the bitmap so we can generate our heightmap
  hm.width = bitmapInfoHeader.biWidth;
  hm.length = bitmapInfoHeader.biHeight;

  // now calculate the size of the bitmap in bytes
  // use 3 as the multiplier (1 each for RGB) even if grayscale image
  int bitmapSize = hm.width * hm.length * 3;

  // create an array to store the bitmap data
  unsigned char* bitmap = new unsigned char[bitmapSize];

  // move the pointer to the beginning of the actual image
  // SEEK_SET = beginning of file
  fseek(filepointer, bmFileHd.bfOffBits, SEEK_SET);

  // read the bitmap image into our bitmap array
  fread(bitmap, 1, bitmapSize, filepointer);

  // close the bitmap file
  fclose(filepointer);

  // now we will transfer the data into our heightmap struct
  hm.heightmap= new XMFLOAT3[hm.width * hm.length];

  // we decide to use grayscale bitmap, so only need to read one of the RGB
  // components, use this index to keep track of our position
  int bitmapIndex = 0;

  // create index to calculate the correct location to store the height in the
  // heightmap array, since not using 2d array
  int heightIndex;

  // create our heightmap from the bitmap data
  for (int i = 0; i < hm.length; i++) {
    for (int k = 0; k < hm.width; k++) {
      height = bitmap[bitmapIndex];

      // index location = width + 2*length
      heightIndex = k + (hm.length * i);

      // store the position and height data in the heightmap
      hm.heightmap[heightIndex].x = (float)k;
      hm.heightmap[heightIndex].y = (float)height / factor;
      hm.heightmap[heightIndex].z = (float)i;

      // increment to the next pixel (skipping over the other 2 RGB components)
      bitmapIndex += 3;
    }
  }

  // cleanup memory
  delete[] bitmap;

  // heightmap successfully generated
  return true;
}