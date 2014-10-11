#include "pch.h"
#include "MoveObject.h"
#include "Content\CommonFunctions.h"

using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

MoveObject::MoveObject(XMFLOAT3 coord, unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY) {
	
}

void MoveObject::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY, XMFLOAT3 colour, XMFLOAT3 coord)
{
	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX/2+coord.x, sideLengthY/2+coord.y, sideLengthZ/2+coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 + coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 + coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 + coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 - coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 + coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 + coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 - coord.y, sideLengthZ / 2 - coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));

	XMFLOAT3 vPosition = XMFLOAT3(sideLengthX / 2 - coord.x, sideLengthY / 2 + coord.y, sideLengthZ / 2 + coord.z);
	XMFLOAT3 vNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vColour = colour;
	vertices.push_back(VertexPositionNormalColour(vPosition, vNormal, vColour));
}