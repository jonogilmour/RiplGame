#pragma once
#include "pch.h"

struct HeightMapInfo {
	int width;  // width of heightmap
	int length; // length of heightmap
	XMFLOAT3 *heightmap;
	bool loaded = false;
} map;

// generating height map function
HeightMapInfo* HeightMapLoad(char *fname);
