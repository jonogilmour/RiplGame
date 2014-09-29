#pragma once

#include "pch.h"

using namespace DirectX;
namespace Ripl {

	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ShaderCBuffer
	{
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4 ambientColour;
	};

}