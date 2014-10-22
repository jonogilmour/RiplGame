#pragma once

#include "pch.h"
#include "Content\Structures\VertexStructures.h"

using namespace DirectX;
namespace RiplGame {
	//STANLEY
	struct water_storage{
		std::vector<unsigned short> indices;
		std::vector<VertexPositionNormalColour> vertices;
	};
}