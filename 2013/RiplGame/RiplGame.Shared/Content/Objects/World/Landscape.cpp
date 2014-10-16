#include "pch.h"
#include "Landscape.h"
#include "Content\CommonFunctions.h"

// Namespaces just spare us from having to write "RiplGame." before everything
using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
// NOTE: This uses initialiser list syntax, essentially each part is initialised using the initialiser for that type.
// DeviceResources uses a couple of initialisers to complete its initialisation, hence the braces.
Landscape::Landscape(unsigned short sideLengthZ, unsigned short sideLengthX) {
	fillIndices(sideLengthZ, sideLengthX);
	fillVertices(sideLengthZ, sideLengthX, XMFLOAT3(0.0f, 0.3f, 0.3f));
}

// Fills a vertex array for a rectangular landscape
void Landscape::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, XMFLOAT3 colour) {
	HeightMapInfo* hm = new HeightMapInfo;
	HeightMapLoad("heightmap_small.bmp", hm);

	// If you want to use the current HeightMapInfo structure:.
	// Access height at (x, z) with:
	// hm->heightmap[x + (z * hm->length)].y

	// HeightMapInfo converted to 2d array for Jono
	//float** heightmap = new float*[hm->length];
	//for (int i = 0; i < hm->width; ++i){
	//	heightmap[i] = new float[hm->width];
	//}

	// Set heightmap values
	//for (int z = 0; z < hm->length; ++z){
	//	for (int x = 0; x < hm->width; ++x) {
	//		heightmap[z][x] = GetHeightAtLocation(hm, x, z);
	//	}
	//}

	float zPos;
	float height;
	float xPos;

	for (unsigned short z = 0; z < sideLengthZ; z++) {
		zPos = float(z) - float(sideLengthZ - 1) / 2.0f;
		for (unsigned short x = 0; x < sideLengthX; x++) {
			xPos = float(x) - float(sideLengthX - 1) / 2.0f;
			height = GetHeightAtLocation(hm, x, z);
			XMFLOAT3 vPosition = XMFLOAT3(xPos, height, zPos);
			XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 vColour = colour;
			vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));
		}
	}

	vertexCount = vertices.size();

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

// Fills an index array for a rectangular landscape
void Landscape::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX) {
	sideLengthZ--;
	sideLengthX--;

	indexCount = sideLengthZ * sideLengthX * 6;
	indices.reserve(indexCount);

	int x = 0;
	int row;
	for (unsigned short z = 0; z < indexCount; z += 6) {
		indices.push_back(x);
		indices.push_back(sideLengthX + x + 1);
		indices.push_back(sideLengthX + x + 2);
		indices.push_back(x);
		indices.push_back(sideLengthX + x + 2);
		indices.push_back(x + 1);
		x++;
		row = ((z / 6) / (sideLengthX + 1)) + 1;
		if (((x + 1) - (row * (sideLengthX + 1))) == 0) x++;
	}
}

uint32 Landscape::getIndexCount() {
	return indexCount;
}

uint32 Landscape::getVertexCount() {
	return vertexCount;
}

// Returns the location of the first index for this object in the large index array
uint32 Landscape::getStartIndexOffset() {
	return startIndexOffset;
}

// Returns the location of the first vertex for this object in the large vertex array
// This value should be set ass the third argument of DrawIndexed
uint32 Landscape::getStartVertexOffset() {
	return startVertexOffset;
}

void Landscape::setStartIndexOffset(uint32 value) {
	startIndexOffset = value;
}
void Landscape::setStartVertexOffset(uint32 value) {
	startVertexOffset = value;
}