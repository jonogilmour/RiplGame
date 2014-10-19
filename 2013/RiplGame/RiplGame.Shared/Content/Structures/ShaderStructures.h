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

	struct _Material
	{
		_Material() 
			: Emissive( 0.0f, 0.0f, 0.0f, 1.0f )
			, Ambient( 0.1f, 0.1f, 0.1f, 1.0f )
			, Diffuse( 1.0f, 1.0f, 1.0f, 1.0f )
			, Specular( 1.0f, 1.0f, 1.0f, 1.0f )
			, SpecularPower( 128.0f )
		{}

		DirectX::XMFLOAT4   Emissive;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Ambient;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Diffuse;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Specular;
		//----------------------------------- (16 byte boundary)
		float               SpecularPower;
		// Add some padding complete the 16 byte boundary.
		float                 Padding[3];
		//----------------------------------- (16 byte boundary)
	}; // Total:                                80 bytes (5 * 16)

	struct Light
	{
		XMFLOAT4      Position;               // 16 bytes
		//----------------------------------- (16 byte boundary)
		XMFLOAT4      Direction;              // 16 bytes
		//----------------------------------- (16 byte boundary)
		XMFLOAT4      Color;                  // 16 bytes
		//----------------------------------- (16 byte boundary)
		float       SpotAngle;              // 4 bytes
		float       ConstantAttenuation;    // 4 bytes
		float       LinearAttenuation;      // 4 bytes
		float       QuadraticAttenuation;   // 4 bytes
		//----------------------------------- (16 byte boundary)
		int         LightType;              // 4 bytes
		int        Enabled;                // 4 bytes
		int        Padding[2];                // 8 bytes
		//----------------------------------- (16 byte boundary)
	};  // Total:                           // 80 bytes (5 * 16 byte boundary)

	struct ModelCBuffer {
		XMFLOAT4X4 model;
	};

	struct ViewCBuffer {
		XMFLOAT4X4 view;
	};

	struct ProjCBuffer {
		XMFLOAT4X4 projection;
	};

	struct MaterialCBuffer {
		struct Material material;
	};

	struct LightCBuffer {
		//XMFLOAT4 eyePosition;
		XMFLOAT4 ambientColour;
		XMFLOAT4 lightColour; //move this into the light structure
		//Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
	};

}