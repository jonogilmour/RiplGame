#pragma once

#include "pch.h"
#include "Content\Structures\ShaderStructures.h"

namespace RiplGame
{
	// A simple rectangular plane
	class Landscape
	{
	public:
		Landscape(unsigned short sideLengthZ, unsigned short sideLengthX);

	private:
		std::vector<VertexPositionNormalColour> vertices;
		unsigned short* indices;
		XMFLOAT4 defaultColour;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	constantBufferData;
		uint32	indexCount;

		void fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, std::vector <VertexPositionNormalColour> vertices, XMFLOAT3 colour);
		void fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short* indices);
	};
}

