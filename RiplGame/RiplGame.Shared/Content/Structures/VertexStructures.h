#pragma once

#include "pch.h"

using namespace DirectX;
namespace RiplGame {

	/*
	 Extremely basic vertex with position and colour info.
	 Not to be used with lighting setups.
	 */
	struct VertexPositionColor
	{
		XMFLOAT3 pos;
		XMFLOAT3 color;
	};

	// A coloured vertex with a position and normal vector
	struct VertexPositionNormalColour
	{
		VertexPositionNormalColour(){}
		VertexPositionNormalColour(
			XMFLOAT3 p,
			XMFLOAT3 n,
			XMFLOAT3 c)
			: pos(p), normal(n), color(c){}

		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT3 color;
	};

	// A textured vertex with a position and normal vector
	struct VertexPositionNormalTexture
	{
		VertexPositionNormalTexture(){}
		VertexPositionNormalTexture(
			XMFLOAT3 p,
			XMFLOAT3 n,
			XMFLOAT2 t)
			: pos(p), normal(n), texCoord(t){}

		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 texCoord;
	};
}