#define MAX_LIGHTS 2

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2


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
	float3  Padding;        // 12 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:               // 80 bytes ( 5 * 16 )

cbuffer MaterialProperties : register(b4)
{
	_Material Material;
};

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
	uint         LightType;              // 4 bytes
	bool		Enabled;				// 4 bytes
	int2        Padding;                // 8 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)

cbuffer LightProperties : register(b5)
{
	float4 EyePosition;                 // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4 GlobalAmbientColour;               // 16 bytes
	//----------------------------------- (16 byte boundary)
	Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
};  // Total:                           // 672 bytes (42 * 16 byte boundary)

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

	return light.Color * pow(RdotV, Material.SpecularPower);
}

float DoAttenuation( Light light, float d )
{
	return 1.0f / ( light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d );
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

LightingResult PointLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation;
	result.Specular = DoSpecular(light, V, L, N) * attenuation;

	return result;
}

LightingResult DirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);

	return result;
}

float DoSpotCone(Light light, float3 L)
{
	float spotMinAngle = cos(light.SpotAngle);
	float spotMaxAngle = (spotMinAngle + 1.0f) / 2.0f;
	float cosAngle = dot(light.Direction.xyz, L);
	return smoothstep(spotMinAngle, spotMaxAngle, cosAngle);
}

LightingResult SpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
		float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, -L);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
	result.Specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
	float3 V = normalize(EyePosition - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		switch (Lights[i].LightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = DirectionalLight(Lights[i], V, P, N);
		}
			break;
		case POINT_LIGHT:
		{
			result = PointLight(Lights[i], V, P, N);
		}
			break;
		case SPOT_LIGHT:
		{
			result = SpotLight(Lights[i], V, P, N);
		}
			break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}

// Per-pixel color data passed through the pixel shader. CHANGE THIS
struct PixelShaderInput
{
	float4 pos   : SV_POSITION;
	float4 norm  : NORMAL;
	float4 color : COLOR0;
	float4 posWS : TEXCOORD0;
};

// A pass-through function for the (interpolated) color data. CHANGE THIS
float4 main(PixelShaderInput input) : SV_TARGET
{
	LightingResult lit = ComputeLighting(input.posWS, normalize(input.norm));

	float4 emissive = Material.Emissive;
	float4 ambient  = Material.Ambient * GlobalAmbientColour;
	float4 diffuse  = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	float4 finalColor = (emissive + ambient + diffuse + specular) * input.color;

	return finalColor;
}
