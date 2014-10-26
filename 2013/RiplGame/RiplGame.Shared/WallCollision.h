#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"

void wallArray(HeightMapInfo *hm, float waterlevel, unsigned short sideLengthZ, unsigned short sideLengthX, std::list<XMFLOAT3>* wallList);
bool wallCollision(XMFLOAT3* object, float objectr, std::list<XMFLOAT3>* wallList);
