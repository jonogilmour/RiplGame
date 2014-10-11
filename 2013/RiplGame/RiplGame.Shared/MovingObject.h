#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"
#include "Content\Structures\ShaderStructures.h"

namespace RiplGame
{
	class MovingObject
	{
	public:
		MovingObject(XMFLOAT3 Position, unsigned short sideLengthZ, unsigned short sideLengthX, unsigned short sideLengthY);
	private:
		void fillVertices(unsigned short sideLengthZ, unsigned short sideLengthX, XMFLOAT3 colour);
		void fillIndices(unsigned short sideLengthZ, unsigned short sideLengthX);
	};
}