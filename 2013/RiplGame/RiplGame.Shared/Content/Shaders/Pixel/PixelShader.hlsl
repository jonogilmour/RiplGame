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
/*
struct Light
{
	float4      Position;               // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4      Direction;              // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
	//----------------------------------- (16 byte boundary)
	float       SpotAngle;              // 4 bytes
	float       ConstantAttenuation;    // 4 bytes
	float       LinearAttenuation;      // 4 bytes
	float       QuadraticAttenuation;   // 4 bytes
	//----------------------------------- (16 byte boundary)
	int         LightType;              // 4 bytes
	bool        Enabled;                // 4 bytes
	int2        Padding;                // 8 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)*/

cbuffer LightProperties : register(b5)
{
	//float4 EyePosition;                 // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4 ambientColour;               // 16 bytes
	//----------------------------------- (16 byte boundary)
	//Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
};  // Total:                           // 672 bytes (42 * 16 byte boundary)



//  REPLACE THIS
cbuffer ShaderCBuffer : register(b0)
{
	// MVP matrices
	//matrix model;
	//matrix view;
	//matrix projection;

	// Position of the camera
	float4 eyeVector;

	// Ambient lighting
	//float4 ambientColour;

	// Directional Light
	float4 lightVector;
	float4 lightColour;

	// material characteristics
	//Material material;
};

// Per-pixel color data passed through the pixel shader. CHANGE THIS
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 norm : NORMAL;
	float4 color : COLOR0;
	float4 hvector : TEXCOORD0;
	float4 vvector : TEXCOORD1;
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
}*/

// Does blinn-phong lighting (more efficient) REPLACE THIS
float4 calcLightingBP(Material m, float4 ambient_colour, float4 light_colour, float4 surface_normal, float4 view_vector, float4 halfway_vector, float4 light_vector)
{
	float specular_shine = 0.9f;

	float4 Ia = m.Ka * ambient_colour;
	float4 Id = m.Kd * saturate(dot(surface_normal, light_vector));
	float4 Is = m.Ks * pow(saturate(dot(surface_normal, halfway_vector)), m.A);

	return Ia + (Id + Is) * light_colour;
}

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	input.norm = normalize(input.norm);
	input.hvector = normalize(input.hvector);

	return calcLightingBP(material, ambientColour, lightColour, input.norm, input.vvector, input.hvector, -lightVector) * input.color;
}
