#include "pch.h"
#include "MoveObject.h"
#include "Content\CommonFunctions.h"

using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

MoveObject::MoveObject(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY) {
	fillIndices();
	fillVertices(sideLengthZ, sideLengthX, sideLengthY, XMFLOAT3(0, 0, 0));
	
	
}

void MoveObject::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY, XMFLOAT3 colour)
{
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;

	XMFLOAT3 vPosition1 = XMFLOAT3(-sideLengthX / 2, sideLengthY / 2, -sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	XMFLOAT3 vPosition2 = XMFLOAT3(-sideLengthX / 2, sideLengthY / 2, sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	XMFLOAT3 vPosition3 = XMFLOAT3(sideLengthX / 2, sideLengthY / 2, sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	XMFLOAT3 vPosition4 = XMFLOAT3(sideLengthX / 2, sideLengthY / 2, -sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	XMFLOAT3 vPosition5 = XMFLOAT3(-sideLengthX / 2, -sideLengthY / 2, sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition5, vNormal, vColour));

	XMFLOAT3 vPosition6 = XMFLOAT3(-sideLengthX / 2, -sideLengthY / 2, -sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition6, vNormal, vColour));

	XMFLOAT3 vPosition7 = XMFLOAT3(sideLengthX / 2, -sideLengthY / 2, sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition7, vNormal, vColour));

	XMFLOAT3 vPosition8 = XMFLOAT3(sideLengthX / 2, -sideLengthY / 2, -sideLengthZ / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition8, vNormal, vColour));
}

void MoveObject::fillIndices() {


	indexCount = 36;
	indices.reserve(indexCount);

	int x = 0;

	// TOP
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	// FRONT
	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(3);

	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(6);
 
	// BOTTOM
	indices.push_back(4); 
	indices.push_back(5); 
	indices.push_back(6);

	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	// BACK
	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(7);

	indices.push_back(1);
	indices.push_back(7);
	indices.push_back(2);

	// LEFT
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(1);

	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(0);

	// RIGHT
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(6);
}

void MoveObject::incrementIndices(unsigned short amount) {
	for (int x = 0; x < getIndexCount(); x++) {
		indices[x] += amount;
	}
}

uint32 MoveObject::getIndexCount() {
	return indexCount;
}

uint32 MoveObject::getVertexCount() {
	return vertexCount;
}