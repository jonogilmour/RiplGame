#include "pch.h"
#include "MoveObject.h"
#include "Content\CommonFunctions.h"

using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

MoveObject::MoveObject(XMFLOAT3 coord, unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY) {
	fillVertices(sideLengthZ, sideLengthX, sideLengthY, XMFLOAT3(0.1f, 0.3f, 0.3f), coord);
	fillIndices(sideLengthZ, sideLengthX);
	
}

void MoveObject::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY, XMFLOAT3 colour, XMFLOAT3 coord)
{
	XMFLOAT3 vPosition1 = XMFLOAT3(sideLengthX/2+coord.x, sideLengthY/2+coord.y, sideLengthZ/2+coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	XMFLOAT3 vPosition2 = XMFLOAT3(sideLengthX / 2 + coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 - coord.z);

	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	XMFLOAT3 vPosition3 = XMFLOAT3(sideLengthX / 2 + coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 + coord.z);
	
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	XMFLOAT3 vPosition4 = XMFLOAT3(sideLengthX / 2 + coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 - coord.z);

	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	XMFLOAT3 vPosition5 = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 + coord.z);
	
	vertices.push_back(VertexPositionNormalColour(vPosition5, vNormal, vColour));

	XMFLOAT3 vPosition6 = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 + coord.z);

	vertices.push_back(VertexPositionNormalColour(vPosition6, vNormal, vColour));

	XMFLOAT3 vPosition7 = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 - coord.z);
	
	vertices.push_back(VertexPositionNormalColour(vPosition7, vNormal, vColour));

	XMFLOAT3 vPosition8 = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 + coord.z);
	
	vertices.push_back(VertexPositionNormalColour(vPosition8, vNormal, vColour));
}

void MoveObject::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX) {
	sideLengthZ--;
	sideLengthX--;

	indexCount = sideLengthZ * sideLengthX * 6;
	indices = new unsigned short[indexCount];

	int x = 0;
	int row;
	for (unsigned short z = 0; z < indexCount; z += 6) {
		indices[z] = x;
		indices[z + 1] = sideLengthX + x + 1;
		indices[z + 2] = sideLengthX + x + 2;
		indices[z + 3] = x;
		indices[z + 4] = sideLengthX + x + 2;
		indices[z + 5] = x + 1;
		x++;
		row = ((z / 6) / (sideLengthX + 1)) + 1;
		if (((x + 1) - (row * (sideLengthX + 1))) == 0) x++;
	}
}

uint32 MoveObject::getIndexCount() {
	return indexCount;
}

uint32 MoveObject::getVertexCount() {
	return vertexCount;
}