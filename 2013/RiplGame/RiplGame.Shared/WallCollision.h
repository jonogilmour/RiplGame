#include "pch.h"
#include "Content\Structures\VertexStructures.h"

using namespace std;

list<XMFLOAT3> wallArray(HeightMapInfo *hm, float waterlevel, unsigned short sideLengthZ, unsigned short sideLengthX);
bool wallCollision(XMFLOAT3* object, float objectr, list<XMFLOAT3> wallList);
