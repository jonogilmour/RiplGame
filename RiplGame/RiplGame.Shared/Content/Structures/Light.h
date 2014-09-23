#pragma once

#include "pch.h"

using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

struct BasicLight
{
	BasicLight()
	{
		ZeroMemory(this, sizeof(BasicLight));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

