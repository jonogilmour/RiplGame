#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"
#include "Content\Structures\ShaderStructures.h"
using namespace RiplGame;

namespace RiplGame
{
	class Ray
	{
	internal:
		Ray();
		XMFLOAT3 position;
		XMFLOAT3 direction;
	};
}