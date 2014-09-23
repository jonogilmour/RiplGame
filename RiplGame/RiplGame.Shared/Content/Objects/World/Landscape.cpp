#include "pch.h"
#include "Landscape.h"

// Namespaces just spare us from having to write "RiplGame." before everything
using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
// NOTE: This uses initialiser list syntax, essentially each part is initialised using the initialiser for that type.
// DeviceResources uses a couple of initialisers to complete its initialisation, hence the braces.
Landscape::Landscape(unsigned short sideLengthZ, unsigned short sideLengthX) {
	fillVertices(sideLengthZ, sideLengthX, this->vertices, XMFLOAT3(1.0f,1.0f,1.0f));
	fillIndices(sideLengthZ, sideLengthX, this->indices);
}

// Fills a vertex array for a rectangular landscape
void Landscape::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, std::vector <VertexPositionNormalColour> vertices, XMFLOAT3 colour) {
	float zPos;
	float xPos;

	for (unsigned short z = 0; z < sideLengthZ; z++) {
		zPos = z - float(sideLengthZ)/2;
		for (unsigned short x = 0; x < sideLengthX; x++) {
			xPos = x - float(sideLengthZ)/2;
			XMFLOAT3 vPosition = XMFLOAT3(xPos, 0.0f, zPos);
			XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 vColour = colour;
			vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));
		}
	}
}

// Fills an index array for a rectangular landscape
void Landscape::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short* indices) {
	sideLengthZ--; 
	sideLengthX--;

	unsigned int arrayLength = sideLengthZ * sideLengthX * 6;
	indices = (unsigned short*)malloc(arrayLength *sizeof(unsigned short));

	int x = 0;
	int row;
	for (unsigned short z = 0; z < arrayLength; z += 6) {
		indices[z] = x;
		indices[z + 1] = sideLengthX + x + 1;
		indices[z + 2] = sideLengthX + x + 2;
		indices[z + 3] = x;
		indices[z + 4] = sideLengthX + x + 2;
		indices[z + 5] = x + 1;
		x++;
		row = (z / 6) / (sideLengthX + 1) + 1;
		if (x + 1 - row * (sideLengthX + 1) == 0) x++;
	}
}