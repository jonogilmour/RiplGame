#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"
#include "Content\Structures\ShaderStructures.h"

namespace RiplGame
{
	class MoveObject
	{
	public:
		MoveObject::MoveObject(float sideLengthZ, float sideLengthX, float sideLengthY);
		uint32 getIndexCount();
		uint32 getVertexCount();
		void incrementIndices(unsigned short amount);
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
		ShaderCBuffer	constantBufferData;
		uint32	indexCount;
		uint32	vertexCount;
		void MoveObject::fillVertices(float sideLengthZ, float sideLengthX, float sideLengthY, XMFLOAT3 colour);
		void MoveObject::fillIndices();

	};
}