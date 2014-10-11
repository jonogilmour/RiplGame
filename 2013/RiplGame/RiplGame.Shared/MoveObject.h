#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"
#include "Content\Structures\ShaderStructures.h"

namespace RiplGame
{
	class MoveObject
	{
	public:
		MoveObject::MoveObject(XMFLOAT3 coord, unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY);
		uint32 getIndexCount();
		uint32 getVertexCount();
		std::vector<VertexPositionNormalColour> vertices;
		unsigned short* indices;

	private:
		XMFLOAT4 defaultColour;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer;

		// System resources for cube geometry.
		ShaderCBuffer	constantBufferData;
		uint32	indexCount;
		uint32	vertexCount;
		void MoveObject::fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY, XMFLOAT3 colour, XMFLOAT3 coord);
		void MoveObject::fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX);

	};
}