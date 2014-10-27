#pragma once

#include "pch.h"

#define MAX_LIGHTS 8

using namespace DirectX;
namespace RiplGame {

	enum LightType
	{
		DirectionalLight = 0,
		PointLight = 1,
		SpotLight = 2
	};

	struct _Material
	{
		_Material() 
			: Emissive( 0.0f, 0.0f, 0.0f, 1.0f )
			, Ambient( 0.25f, 0.25f, 0.25f, 1.0f )
			, Diffuse( 1.0f, 1.0f, 1.0f, 1.0f )
			, Specular(0.774597f, 0.774597f, 0.774597f, 1.0f)
			, SpecularPower( 76.8f )
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
		unsigned int         LightType;              // 4 bytes
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
		struct _Material material;
	};

	struct LightCBuffer {
		XMFLOAT4 EyePosition;
		XMFLOAT4 GlobalAmbientColour;
		Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
	};

}