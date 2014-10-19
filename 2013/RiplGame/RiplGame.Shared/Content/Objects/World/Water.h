#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"
#include "Content\Structures\ShaderStructures.h"

namespace RiplGame
{
	// A simple rectangular plane
	class Water
	{
	public:
		Water(unsigned short sideLengthZ, unsigned short sideLengthX);
		uint32 getIndexCount();
		uint32 getVertexCount();
		uint32 getStartIndexOffset();
		uint32 getStartVertexOffset();
		void setStartIndexOffset(uint32 value);
		void setStartVertexOffset(uint32 value);
		std::vector<VertexPositionNormalColour> vertices;
		std::vector<unsigned short> indices;

	private:
		XMFLOAT4 defaultColour;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer;

		// System resources for cube geometry.
		uint32	indexCount;
		uint32	vertexCount;
		uint32 startIndexOffset;
		uint32 startVertexOffset;

		void fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, XMFLOAT4 colour);
		void fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX);
	};
}

