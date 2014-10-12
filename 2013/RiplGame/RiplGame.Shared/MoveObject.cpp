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

	XMFLOAT3 vPosition8 = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 - coord.z);
	
	vertices.push_back(VertexPositionNormalColour(vPosition8, vNormal, vColour));
}

void MoveObject::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX) {


	indexCount = 36;
	indices = new unsigned short[indexCount];

	int x = 0;
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 1;
		indices[4] = 3;
		indices[5] = 2;
		//right hand face

		indices[6] = 5;
		indices[7] = 1;
		indices[8] = 6;

		indices[9] = 1;
		indices[10] = 3;
		indices[11] = 6;
		//front face

		indices[12] = 5;
		indices[13] = 6;
		indices[14] = 7;

		indices[15] = 5;
		indices[16] = 7;
		indices[17] = 8;
		//left face

		indices[18] = 8;
		indices[19] = 7;
		indices[20] = 4;

		indices[21] = 8;
		indices[22] = 2;
		indices[23] = 4;
		//rear face

		indices[24] = 8;
		indices[25] = 5;
		indices[26] = 2;
		indices[27] = 5;
		indices[28] = 1;
		indices[29] = 2;
		//top face

		indices[30] = 6;
		indices[31] = 3;
		indices[32] = 4;

		indices[33] = 6;
		indices[34] = 7;
		indices[35] = 2;
		//bottom face
 
	
}

uint32 MoveObject::getIndexCount() {
	return indexCount;
}

uint32 MoveObject::getVertexCount() {
	return vertexCount;
}