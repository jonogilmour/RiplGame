/*
HERE'S WHERE THINGS GET INTERESTING

Now a shader is essentially a little program that displays stuff.
Without shaders you would not have any lighting, movement, or even rendering of any vertices.
Remember that vertices are just numbers, until the shader takes them and turns them into coloured lines or shapes.
So essentially, shaders are the backbone of any display of anything ever.

Now, shaders are extremely complicated to create, because they are all machine code. Fortunately, the
good folks at microshit have given DirectX a language called HLSL (google it). HLSL isn't C or anything else, it is
a completely separate language.

HLSL lets you code shaders simply and easily using a syntax that's a lot like C (I think you'll agree). The key to shaders
is to make them as small as possible; in fact there is an imposed size restriction for shaders, so you want them to do 
as much as possible in as small a space as possible.

Now let's just go into things for a second:

VERTEX SHADERS
Vertex shaders are where the majority of the magic happens. They are responsible for setting up
and rendering your vertices, which are what 3D object are made from.
Now the vertex shader incorporates stuff like the location of the vertices, and the transformation
using the view/projection/world (or model) matrices.

The colour here is actually dealt with by the pixel shaders, which are in another HLSL file (check it out).
But the colour values are passed from the vertex shaders to the pixel shaders because *drum roll please*

...the vertex shaders do ALL THE LIGHTING. That's right, in SharpDX and XNA, the BasicEffect class is actually a bunch of 
preprogrammed shaders for you to use. So here, we need to custom code the lighting, the reflections (normals go here too)
and all that good stuff.

Now shaders do shit all by themselves without DATA. You'll notice no data gets passed around here. All this file is, is
a specification of the STRUCTURE of the shader, plus a main function that implements the shader logic. The shader gets its data when
you fill it using the cpp files like the SceneRenderer (check that out and read the comments to see where this happens). When this
data comes in, it is stored in the structures below, and the structures are used to store this data for use in the main function below.

So how does the app know where to put the data? How does it know that "pos" in the shader is where the position vectors go? Using SEMANTICS.
The "POSITION" and "COLOR0" lines are essentially identifiers for each field. These are specific, you can't just put any old string here, there's
a list of legal ones on microshit's website.

Go check out the SceneRenderer, and you'll see where the POSITION and COLOR strings are used (and the locations in the VertexPositionColor structs
where the shader gets these values).

The basic process is:
- Compile the app, and these hlsl files are turned into super-fast shader object code files, called CSO files
- Read these CSO files in, and you get the shaders
- Tell the GPU "Yo dawg, these are the shaders right here, and here is the data for them that I made"
- The data comes in as 1s and 0s, so the shader has no clue what they mean
- You tell them what they mean, by saying "This part is the POSITION, this part is the COLOR"
- The shader reads the values, and can then create and apply shit to the vertices
- The MVP matrices also get made, and are stored in a CONSTANT BUFFER, because they aren't shader specific and should be shared amongst ALL SHADERS for an object!
- The vertex shader (the one in this file) sets up where everything is and what colour it should be
- This data is passed to the PIXEL SHADER (the next file). Things always happen in this order: Vertex Shader -> Pixel Shader
- The Pixel shader then does the job of mapping all the 3D shit into a flat image to display on your compoooooter screen
- The pixel shader works on colour values, that's it really, because a pixel is just a colour. That's why it's much smaller than this vertex shader
- Everything is coloured and in place, then it is rendered.

*/


float3 ambientColour = float3(1.0f, 1.0f, 1.0f);
float ambientIntensity = 0.4;

// A constant buffer that stores the three basic column-major matrices for composing geometry.
// The "register(b0)" part sets the register to expect constant buffer data to be in. You fill
// this using the
/*
	VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);
*/
// function. The first parameter specifies which register to save to.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// Pass the color through without modification.
	output.color = input.color;

	return output;
}
