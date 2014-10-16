#pragma once
#include "pch.h"

struct HeightMapInfo {
	int width;  // width of heightmap
	int length; // length of heightmap
	XMFLOAT3 *heightmap;
	bool loaded = false;
} map;

// generating height map function
void HeightMapLoad(char *fname, HeightMapInfo *hm);

// get height at location (x, z)
float GetHeightAtLocation(HeightMapInfo *hm, int x, int z);
