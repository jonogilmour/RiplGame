#pragma once

#include "pch.h"
#include "Structures\VertexStructures.h"
#include "Structures\ShaderStructures.h"

namespace Ripl
{
	// A simple rectangular plane
	class Landscape
	{
	public:
		Landscape(unsigned short sideLengthZ, unsigned short sideLengthX);
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

		void fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, XMFLOAT3 colour);
		void fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX);
	};
}

