#define MAX_LIGHTS 8

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// REPLACE THIS
struct Material
{
	float Ka, Kd, Ks, A;
};
/*
struct _Material
{
	float4  Emissive;       // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4  Ambient;        // 16 bytes
	//------------------------------------(16 byte boundary)
	float4  Diffuse;        // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4  Specular;       // 16 bytes
	//----------------------------------- (16 byte boundary)
	float   SpecularPower;  // 4 bytes
	float3  Padding;        // 8 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:               // 80 bytes ( 5 * 16 )*/

cbuffer MaterialProperties : register(b4)
{
	Material material;
};

struct Light
{
	//float4      Position;               // 16 bytes
	//----------------------------------- (16 byte boundary)
	//float4      Direction;              // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
	//----------------------------------- (16 byte boundary)
	//float       SpotAngle;              // 4 bytes
	//float       ConstantAttenuation;    // 4 bytes
	//float       LinearAttenuation;      // 4 bytes
	//float       QuadraticAttenuation;   // 4 bytes
	//----------------------------------- (16 byte boundary)
	//int         LightType;              // 4 bytes
	//int3        Padding;                // 12 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)

cbuffer LightProperties : register(b5)
{
	//float4 EyePosition;                 // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4 ambientColour;               // 16 bytes
	float4 lightColour;
	//----------------------------------- (16 byte boundary)
	Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
};  // Total:                           // 672 bytes (42 * 16 byte boundary)

// Per-pixel color data passed through the pixel shader. CHANGE THIS
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 norm : NORMAL;
	float4 color : COLOR0;
};
/*
float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Phong lighting.
	float3 R = normalize(reflect(-L, N));
		float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	float3 H = normalize(L + V);
		float NdotH = max(0, dot(N, H));

	return light.Color * pow(RdotV, Material.SpecularPower);
}

float DoAttenuation( Light light, float d )
{
	return 1.0f / ( light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d );
}
*/

// A pass-through function for the (interpolated) color data. CHANGE THIS
float4 main(PixelShaderInput input) : SV_TARGET
{
	return input.color;
}
