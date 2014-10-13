struct Material
{
	float Ka, Kd, Ks, A;
};

cbuffer ShaderCBuffer : register(b0)
{
	// MVP matrices
	matrix model;
	matrix view;
	matrix projection;

	// Position of the camera
	float4 eyeVector;

	// Ambient lighting
	float4 ambientColour;

	// Directional Light
	float4 lightVector;
	float4 lightColour;

	// material characteristics
	Material material;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 norm : NORMAL;
	//float4 color : COLOR0;
	float4 hvector : TEXCOORD0;
	float4 vvector : TEXCOORD1;
};

// Does blinn-phong lighting (more efficient)
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

	return calcLightingBP(material, ambientColour, lightColour, input.norm, input.vvector, input.hvector, -lightVector);
}
