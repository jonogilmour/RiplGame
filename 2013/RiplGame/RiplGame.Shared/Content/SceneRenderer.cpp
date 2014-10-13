// pch.h is a shortcut for importing everything that's needed by any of the files in this project. See pch.h for details
#include "pch.h"
#include "SceneRenderer.h"
#include "Content\Objects\World\Landscape.h"
#include "MoveObject.h"

// Namespaces just spare us from having to write "RiplGame." before everything
using namespace RiplGame;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
// NOTE: This uses initialiser list syntax, essentially each part is initialised using the initialiser for that type.
// DeviceResources uses a couple of initialisers to complete its initialisation, hence the braces.
SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
m_loadingComplete(false),
m_indexCount(0),
m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	// Update all controller events
	m_controller->Update(CoreWindow::GetForCurrentThread(), timer.GetElapsedSeconds());

	XMVECTOR eye = XMLoadFloat3(&m_controller->get_Position());
	XMVECTOR at = XMLoadFloat3(&m_controller->get_LookAt());
	XMVECTOR up = XMLoadFloat3(&m_controller->get_UpAxis());

	// Setup the constant buffer
	m_constantBufferData.ambientColour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_constantBufferData.lightVector = XMFLOAT4(0, -0.0001f, 0, 0);
	m_constantBufferData.lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMStoreFloat4(&m_constantBufferData.eyeVector, eye);
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
}

// Renders one frame using the vertex and pixel shaders.
void SceneRenderer::Render()
{
	//REMEMBER: Asynchronous means it continues in the background and doesn't block

	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	// auto keyword infers the type of the variable
	// The context just grabs the settings of the graphics device
	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	// A constant buffer is just a set of data that all shaders will need to refer to, like a common data set
	// The constant buffer is filled with data beforehand, see the parts above where the view/projection matrices were made
	// notice that they are put into the constant buffer because every shader needs to know about these matrices
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	// This just stores the index and vertex buffers to be sent to the graphics card
	UINT stride = sizeof(VertexPositionNormalColour);
	UINT offset = 0;
	// Vertexbuffer
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);
	// Indexbuffer
	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	// Sets what the vertexes will draw, in this case it is a strip of triangles (a common configuration)
	// This means that the GPU expects that the vertex buffer specifies a bunch of triangles
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// The input layout essentially tells the shaders what the vertex structs mean
	// As in, the first field of the vertexpositioncolor struct is the position vector,
	// and the second is the colour of the vector.
	//
	// This information essentially tells the shader which pieces of data to use to do what, like what field
	// in the struct is used to set the vertex position, and which is used to set the colour.
	//
	// This is because all the shader sees is bits, so it needs to know what those numbers mean.
	// For example, if you reversed the input layout and made the first field colour and the second field position,
	// all of the position info would be used by the shader to set the colour, and vice versa for the colour.
	context->IASetInputLayout(m_inputLayout.Get());

	// Grabs the vertex shader and prepares it to be sent to the GPU
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Does the same to the constant buffer
	// BIG NOTE!!!!:::::: The first number in this function specifies which register to stick this constant buffer into
	// The '0' here means stick it in register b0 on the GPU. You'll notice in the vertex shader, the MVP struct is saved
	// in register(b0), meaning that if you want to make another constant buffer and save it to register b1, you should
	// set the first argument in VSSetConstantBuffers to 1.
	// Try it out, change the 0 below to a 1, then try running the program. Nothing will come up because the MVP matrices have not been saved
	// Then, try changing the register value in the vertex shader to b1, then run. It should work now, because you've mapped the buffers correctly
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// And the pixel shader
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	/* Shaders and buffers set. Begin draw calls */
	for (int x = 0; x < staticObject_IndexCount.size(); x++) {
		// First, set the model matrix to render the static object and update the constant buffer
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));
		context->UpdateSubresource(
			m_constantBuffer.Get(),
			0,
			NULL,
			&m_constantBufferData,
			0,
			0
			);

		// Draw the object
		context->DrawIndexed(
			staticObject_IndexCount[x],
			staticObject_StartIndexOffset[x],
			staticObject_StartVertexOffset[x]
			);
	}
	
	for (int x = 0; x < dynamicObject_IndexCount.size(); x++) {
		// First, set the model matrix to render the static object and update the constant buffer
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixTranslation(0,3,0)));
		context->UpdateSubresource(
			m_constantBuffer.Get(),
			0,
			NULL,
			&m_constantBufferData,
			0,
			0
			);

		// Draw the object
		context->DrawIndexed(
			dynamicObject_IndexCount[x],
			dynamicObject_StartIndexOffset[x],
			dynamicObject_StartVertexOffset[x]
			);
	}
}

void SceneRenderer::CreateDeviceDependentResources()
{
	// Setup the keyboard/mouse controller
	m_controller = ref new MoveLookController();
	m_controller->Initialize(CoreWindow::GetForCurrentThread());

	//REMEMBER: Asynchronous means it continues in the background and doesn't block
	// Load shaders asynchronously on separate threads, so they can both be loaded at the same time
	// Note that these .cso files are generates from the .hlsl shader files on the right, they're like the
	// shader byte-code
	auto loadVSTask = DX::ReadDataAsync(L"VertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	// The ".then" syntax means "wait for X to finish doing its thing, then execute the following code"
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		// fileData contains the results of loadVStask, ie the shader code
		// This creates the vertex shader. Throwiffailed because without a vertex shader we cant do SHIT
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
			&fileData[0],
			fileData.size(),
			nullptr,
			&m_vertexShader
			)
			);

		// Aligns the vertexpositionnormalcolor struct's fields with the semantic fields in the shader. 
		// This pulls out the data from each field and uses it as input to the shader, working on byte-sized blocks.
		// The "dxgi" part refers to the size of the variable, these being 3x4 byte floats.
		// The 5th arg refers to the offset within the vertex structure where this field resides. The "position" 
		// is the first field in the struct (so offset = 0), and the "normal" field is 12 bytes along (being a 3x4 byte float), 
		// and so on.
		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// Creates the vertex input layout using the description array above
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			&fileData[0],
			fileData.size(),
			&m_inputLayout
			)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	// Again, we wait until the pixel shader file is completely loaded in, then...
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		// Same shit as before, grab the contents of the pixel shader file and make a pixel shader out of it
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
			&fileData[0],
			fileData.size(),
			nullptr,
			&m_pixelShader
			)
			);
		// Make a constant buffer, filled with the information about the model, view, and projection matrices stored in constantBufferDesc
		// This constant buffer is then saved in m_constantBuffer
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ShaderCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&m_constantBuffer
			)
			);
	});

	// Once both shaders are loaded, create the mesh.
	// Notice how we are &&ing the two tasks, which means we wait for both to completely finish, then we do...
	auto createLandscapeTask = (createPSTask && createVSTask).then([this]() {

		Landscape landscape(10, 10);
		MoveObject moveObject(0.5f,0.5f,0.5f);

		// This creates the data (vertices) to put into the vertex buffer, and zeroes it
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		std::vector<VertexPositionNormalColour> vertices;

		vertices.reserve(landscape.getVertexCount() + moveObject.getVertexCount()); // preallocate memory
		vertices.insert(vertices.end(), landscape.vertices.begin(), landscape.vertices.end());
		vertices.insert(vertices.end(), moveObject.vertices.begin(), moveObject.vertices.end());

		// pSysMem is a pointer to the data to put in
		vertexBufferData.pSysMem = &(vertices[0]);
		// This is only used for textures, so make it 0 as we arent using textures here
		vertexBufferData.SysMemPitch = 0;
		// Ditto for this
		vertexBufferData.SysMemSlicePitch = 0;

		// Here we actually create the vertex buffer. It's length is the size of the vertex array
		// Note this isn't a function, it's an initialiser for a variable called vertexBufferDesc

		int vertexCount = (landscape.getVertexCount() + moveObject.getVertexCount())*sizeof(VertexPositionNormalColour);
		CD3D11_BUFFER_DESC vertexBufferDesc(vertexCount, D3D11_BIND_VERTEX_BUFFER);
		// And here we send it to the device like we did with the shaders
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
			)
			);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.

		//static const unsigned short* indices = landscape.indices;
		std::vector<unsigned short> indices;


		/////////////////////////////
		/* SETUP THE MODEL INDICES */
		/////////////////////////////

		/* 
		In this section, every object's indices must be setup in the order you wish the render them. 
		
		This order must be preserved when calling the DrawIndexed() function to draw the objects in the Render() method.
		
		This allows objects to be moved independently of each other, by setting and resetting the world matrix before 
		and after every draw call, respectively.
		*/

		//STATIC OBJECTS
		// Insert the landscape
		staticObject_StartIndexOffset.push_back(indices.size());
		staticObject_StartVertexOffset.push_back(0);
		staticObject_IndexCount.push_back(landscape.getIndexCount());
		indices.insert(indices.end(), landscape.indices.begin(), landscape.indices.end());

		//DYNAMIC OBJECTS
		// Insert the moving object
		dynamicObject_StartIndexOffset.push_back(indices.size());
		dynamicObject_StartVertexOffset.push_back(landscape.getVertexCount());
		dynamicObject_IndexCount.push_back(moveObject.getIndexCount());
		indices.insert(indices.end(), moveObject.indices.begin(), moveObject.indices.end());
		
		////////////////////////
		/* MODEL INDICES DONE */
		////////////////////////


		// Store the length of the index array
		m_indexCount = landscape.getIndexCount() + moveObject.getIndexCount();

		// Do the same thing as we did for the vertex buffer to set the index buffer
		// First zero it
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };

		// then tell it where the data is
		indexBufferData.pSysMem = (&indices[0]);
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		// Now push the index buffer to the graphics card
		CD3D11_BUFFER_DESC indexBufferDesc(m_indexCount*sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
			)
			);
	});

	// Once the cube is loaded, the object is ready to be rendered.
	// Setting this to true tells the renderer that all is well and shit's ready to be rendered
	createLandscapeTask.then([this]() {
		m_loadingComplete = true;
	});
}

void SceneRenderer::ReleaseDeviceDependentResources()
{
	// this just resets everything in the device to default (normally 0)
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

// Initializes view parameters when the window size changes.
void SceneRenderer::CreateWindowSizeDependentResources()
{
	//Grab the size of the window
	Size outputSize = m_deviceResources->GetOutputSize();
	// Calculate the aspect ratio
	float aspectRatio = outputSize.Width / outputSize.Height;
	// Calculate the vertical FOV, note that this is 70 degrees here
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	// Construct the projection matrix as per usual
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	// Orientation is the orientation of a monitor, ie landscape or portrait
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	// The orientation matrix automatically transforms the perspective according to the orientation.
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	// This combines the projection and orientation matrices into the final projection matrix.
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	//By the end of this method, we have the view and proj matrices saved into the constant buffer
	//Just need the model/world matrix and we are good to go (see Rotate() for this)
}