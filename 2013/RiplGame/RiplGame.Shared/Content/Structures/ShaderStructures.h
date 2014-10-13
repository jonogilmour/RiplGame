#pragma once

#include "pch.h"

using namespace DirectX;
namespace RiplGame {

	struct Material
	{
		Material() { Ka = Kd = Ks = A = 0; }
		Material(float ka, float kd, float ks, float a) :
			Ka(ka),
			Kd(kd),
			Ks(ks),
			A(a)
		{}

		float Ka, Kd, Ks, A;
	};

	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ShaderCBuffer
	{
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4 eyeVector;
		XMFLOAT4 ambientColour;
		XMFLOAT4 lightVector;
		XMFLOAT4 lightColour;
		struct Material material;
	};

}