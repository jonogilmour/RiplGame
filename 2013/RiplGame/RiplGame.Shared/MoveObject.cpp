#include "pch.h"
#include "MoveObject.h"
#include "Content\CommonFunctions.h"

using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

MoveObject::MoveObject(float sideLengthZ, float sideLengthX, float sideLengthY) {
	fillIndices();
	fillVertices(sideLengthZ, sideLengthX, sideLengthY, XMFLOAT3(0, 0, 0));


}

void MoveObject::fillVertices(float sideLengthZ, float sideLengthX, float sideLengthY, XMFLOAT3 colour)
{
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;

	vertexCount = 24;

	// TOP
	XMFLOAT3 vPosition1 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	XMFLOAT3 vPosition2 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	XMFLOAT3 vPosition3 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	XMFLOAT3 vPosition4 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));
	
	

	// LEFT
	vPosition1 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	vPosition2 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	vPosition3 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	vPosition4 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	// RIGHT
	vPosition1 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	vPosition2 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	vPosition3 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	vPosition4 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	// BACK
	vPosition1 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	vPosition2 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	vPosition3 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	vPosition4 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	// FRONT
	vPosition1 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	vPosition2 = XMFLOAT3(-float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	vPosition3 = XMFLOAT3(float(sideLengthX) / 2, float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	vPosition4 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	// BOTTOM
	vPosition1 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition1, vNormal, vColour));

	vPosition2 = XMFLOAT3(-float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition2, vNormal, vColour));

	vPosition3 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition3, vNormal, vColour));

	vPosition4 = XMFLOAT3(float(sideLengthX) / 2, -float(sideLengthY) / 2, -float(sideLengthZ) / 2);
	vertices.push_back(VertexPositionNormalColour(vPosition4, vNormal, vColour));

	// Calculate and average the normals
	for (unsigned int x = 0; x < getIndexCount() / 3; x++) {
		unsigned short index1 = indices[x * 3];
		unsigned short index2 = indices[x * 3 + 1];
		unsigned short index3 = indices[x * 3 + 2];

		XMFLOAT3 side1 = XMFLOAT3(vertices[index2].pos.x - vertices[index1].pos.x, vertices[index2].pos.y - vertices[index1].pos.y, vertices[index2].pos.z - vertices[index1].pos.z);
		XMFLOAT3 side2 = XMFLOAT3(vertices[index3].pos.x - vertices[index1].pos.x, vertices[index3].pos.y - vertices[index1].pos.y, vertices[index3].pos.z - vertices[index1].pos.z);
		XMFLOAT3 normal = cross(side1, side2);

		vertices[index1].normal.x += normal.x;
		vertices[index1].normal.y += normal.y;
		vertices[index1].normal.z += normal.z;
		vertices[index2].normal.x += normal.x;
		vertices[index2].normal.y += normal.y;
		vertices[index2].normal.z += normal.z;
		vertices[index3].normal.x += normal.x;
		vertices[index3].normal.y += normal.y;
		vertices[index3].normal.z += normal.z;
	}

	// Normalise all the normal vectors
	for (unsigned int x = 0; x < getVertexCount(); x++) normalizeF3(&vertices[x].normal);
}

void MoveObject::fillIndices() {


	indexCount = 36;
	indices.reserve(indexCount);

	for (int x = 0; x < 20; x += 4) {
		indices.push_back(x);
		indices.push_back(x + 1);
		indices.push_back(x + 2);

		indices.push_back(x);
		indices.push_back(x + 2);
		indices.push_back(x + 3);
	}

	// BOTTOM
	indices.push_back(20);
	indices.push_back(22);
	indices.push_back(21);

	indices.push_back(20);
	indices.push_back(23);
	indices.push_back(22);
	
}

uint32 MoveObject::getIndexCount() {
	return indexCount;
}

uint32 MoveObject::getVertexCount() {
	return vertexCount;
}

// Returns the location of the first index for this object in the large index array
uint32 MoveObject::getStartIndexOffset() {
	return startIndexOffset;
}

// Returns the location of the first vertex for this object in the large vertex array
// This value should be set ass the third argument of DrawIndexed
uint32 MoveObject::getStartVertexOffset() {
	return startVertexOffset;
}

void MoveObject::setStartIndexOffset(uint32 value) {
	startIndexOffset = value;
}
void MoveObject::setStartVertexOffset(uint32 value) {
	startVertexOffset = value;
}