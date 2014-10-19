cbuffer viewMatrix : register(b1)
{
	matrix view;
}

cbuffer projMatrix : register(b2)
{
	matrix projection;
}

cbuffer modelMatrix : register(b3)
{
	matrix model;
}

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float3 color : COLOR0;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 norm : NORMAL;
	float4 color : COLOR0;
};



// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);
	float4 normal = float4(input.norm, 0);
	float4 color = float4(input.color, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// Translate the normal
	normal = mul(normal, model);
	output.norm = normal;

	output.color = color;

	return output;
}