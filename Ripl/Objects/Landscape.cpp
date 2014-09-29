#include "pch.h"
#include "Landscape.h"

// Namespaces just spare us from having to write "RiplGame." before everything
using namespace Ripl;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
// NOTE: This uses initialiser list syntax, essentially each part is initialised using the initialiser for that type.
// DeviceResources uses a couple of initialisers to complete its initialisation, hence the braces.
Landscape::Landscape(unsigned short sideLengthZ, unsigned short sideLengthX) {
	fillVertices(sideLengthZ, sideLengthX, XMFLOAT3(0.0f,0.0f,0.0f));
	fillIndices(sideLengthZ, sideLengthX);
}

// Fills a vertex array for a rectangular landscape
void Landscape::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, XMFLOAT3 colour) {
	float zPos;
	float xPos;

	for (unsigned short z = 0; z < sideLengthZ; z++) {
		zPos = float(z) - float(sideLengthZ-1)/2.0f;
		for (unsigned short x = 0; x < sideLengthX; x++) {
			xPos = float(x) - float(sideLengthX-1)/2.0f;
			XMFLOAT3 vPosition = XMFLOAT3(xPos, 0.0f, zPos);
			XMFLOAT3 vNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			XMFLOAT3 vColour = colour;
			vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));
		}
	}
	//Copy the contents of the vertex vector to array form
	vertexCount = vertices.size();
}

// Fills an index array for a rectangular landscape
void Landscape::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX) {
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

uint32 Landscape::getIndexCount() {
	return indexCount;
}

uint32 Landscape::getVertexCount() {
	return vertexCount;
}