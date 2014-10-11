#pragma 
#include "pch.h"

struct HeightMapInfo {
	int width;
	int length;
	XMFLOAT3 *heightmap;
};


ref class HeightMap {
private:

public:

	// load the bitmap into our heightmap structure
	// returns true if succesful, false otherwise
	bool LoadHeightmapBitmap(char* fname, HeightMapInfo &hm);
};