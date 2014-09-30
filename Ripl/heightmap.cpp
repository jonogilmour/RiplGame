// Stores the height, width, and array of vertices
struct HeightMapInfo {
  int width; // width of heightmap
  int length; // length of heightmap
  XMFLOAT3 *heightmap; // stores (x, y, z) positions where y is bitmap height
};

void LoadHeightmapBitmap(char* fname, HeightMapInfo &hm){
  FILE *filepointer;
  BITMAPINFOHEADER bmInfoHeader; // info about bitmap
  BITMAPFILEHEADER bmFileHeader; // info about the bitmap file


}