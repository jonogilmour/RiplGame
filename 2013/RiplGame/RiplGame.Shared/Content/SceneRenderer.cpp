// pch.h is a shortcut for importing everything that's needed by any of the files in this project. See pch.h for details
#include "pch.h"
#include "SceneRenderer.h"
#include "Content\Objects\World\Landscape.h"
#include "Content\Objects\World\Water.h"
#include "MoveObject.h"
#include "WallCollision.h"



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
	GameStarted = false;
	gameWon = false;
	gameEnded = false;
	succeeds = 0;
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	if (gameWon) {
		if (staticObject_Transforms[1]._24 < 4) {
			staticObject_Transforms[1]._24 += timer.GetElapsedSeconds() * 2.0f;
		}
	}

	else if (gameEnded) {
		if (staticObject_Transforms[1]._24 > -4) {
			staticObject_Transforms[1]._24 -= timer.GetElapsedSeconds() * 2.0f;
		}
	}

	else {
		XMFLOAT4 CubePos;

		Size outputSize = m_deviceResources->GetOutputSize();
		// check if not 1st tick
		// cube is at position 0 in dynamicObject
		// dynamicObject_Transforms is the world matrix
		// to move ANY object at all, take each vertex * worldMatrix and it moves the entire object
		// view moves world around camera
		// projection distorts all vertices so that it looks normal - like an eye would see (FOV, aspect ratio etc.) - created in WindowSize... - projectionMatrix is always constant
		// ws - water storage
		// controller update - movement of camera and objects
		if (!(dynamicObject_Transforms.size() < 1))	{
			m_controller->Update(CoreWindow::GetForCurrentThread(), timer.GetElapsedSeconds(), &dynamicObject_Transforms[current_game_info.current_life], outputSize, m_constantBufferData_View.view, m_constantBufferData_Proj.projection, &wallList);
			CubePos = XMFLOAT4(dynamicObject_Transforms[current_game_info.current_life]._14, dynamicObject_Transforms[current_game_info.current_life]._24, dynamicObject_Transforms[current_game_info.current_life]._34, 1);
		}
		else {
			m_controller->Update(CoreWindow::GetForCurrentThread(), timer.GetElapsedSeconds(), nullptr, outputSize, m_constantBufferData_View.view, m_constantBufferData_Proj.projection, &wallList);
			CubePos = XMFLOAT4(0, 0, 0, 1);
		}

		// camera stuff
		XMVECTOR eye = XMLoadFloat3(&m_controller->get_Position()); // position of camera in world
		XMVECTOR at = XMLoadFloat3(&m_controller->get_LookAt());   // what point is the camera looking at
		XMVECTOR up = XMLoadFloat3(&m_controller->get_UpAxis());   // which direction is up, important for getting things in correct perspective

		// Store the eye position
		// convert eye to XMFLOAT4
		XMStoreFloat4(&m_constantBufferData_Light.EyePosition, eye);

		// Setup the constant buffer
		XMStoreFloat4x4(&m_constantBufferData_View.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

		// Set up lights

		// Do lights (directional)
		Light light;
		light.Enabled = 1;
		light.LightType = DirectionalLight;
		light.Color = XMFLOAT4(1, 1, 1, 1);
		light.SpotAngle = XMConvertToRadians(45.0f);
		light.ConstantAttenuation = 1.0f;
		light.LinearAttenuation = 0.08f;
		light.QuadraticAttenuation = 0.0f;
		light.Position = XMFLOAT4(0, 0, 0, 1);
		XMVECTOR LightDirection = XMVectorSet(10, -5, 10, 0.0f);
		LightDirection = XMVector3Normalize(LightDirection);
		XMStoreFloat4(&light.Direction, LightDirection);

		// Store the light
		//m_constantBufferData_Light.Lights[0] = light;

		// Do lights (point)
		Light light2;
		light2.Enabled = 1;
		light2.LightType = PointLight;
		light2.Color = XMFLOAT4(1, 1, 1, 1);
		light2.SpotAngle = XMConvertToRadians(45.0f);
		light2.ConstantAttenuation = 0.0f;
		light2.LinearAttenuation = 0.2f;
		light2.QuadraticAttenuation = 0.05f;
		light2.Position = XMFLOAT4(CubePos.x, CubePos.y + 3, CubePos.z, 1.0);
		LightDirection = XMVectorSet(0, -1, 0, 0.0f);
		LightDirection = XMVector3Normalize(LightDirection);
		XMStoreFloat4(&light2.Direction, LightDirection);

		// Store the light
		m_constantBufferData_Light.Lights[0] = light2;

		// Do wall collisions for the object and increment
		XMFLOAT3 cubeCentre(CubePos.x, CubePos.y, CubePos.z);
		if (wallCollision(&cubeCentre, 0.5, &wallList)) {
			if (cubeCentre.x > 26.5) {
				succeeds++;
				if (succeeds == current_game_info.target) {
					gameWon = true;
				}
			}

			// Cube has hit a wall. Freeze it and spawn a new one at the base point
			current_game_info.current_life++;
			if (current_game_info.current_life >= current_game_info.max_lives && !gameWon) {
				// Ran out of lives
				// GAME.END
				gameEnded = true;
				return;
				// fatal error for now!
				//int x = 0;
				//int y = 1 / x;
			}

			// Add previous cube to collisions list
			wallList.push_back(cubeCentre);

			XMFLOAT3 targetPosition(-20, 13, -28);
			// Still have lives left, spawn a new cube and move camera to start
			m_controller->moveCameraToLocation(targetPosition, XMFLOAT3(-28, 4, -28), true);
		}
	}
}

// Renders one frame using the vertex and pixel shaders.
void SceneRenderer::Render()
{
	// REMEMBER: Asynchronous means it continues in the background and doesn't block

	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	// auto keyword infers the type of the variable
	// The context just grabs the settings of the graphics device
	// Gives functions access to shaders and buffers
	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	// A constant buffer is just a set of data that all shaders will need to refer to, like a common data set
	// The constant buffer is filled with data beforehand, see the parts above where the view/projection matrices were made
	// notice that they are put into the constant buffer because every shader needs to know about these matrices
	// Update graphics card with this data
	context->UpdateSubresource( // view update
		m_constantBuffer_View.Get(), // structure given to shader to communicate
		0,
		NULL,
		&m_constantBufferData_View,
		0,
		0
		);
	context->UpdateSubresource( // projection update
		m_constantBuffer_Proj.Get(),
		0,
		NULL,
		&m_constantBufferData_Proj,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	// This just stores the index and vertex buffers to be sent to the graphics card
	UINT stride = sizeof(VertexPositionNormalColour);
	UINT offset = 0;
	// Vertexbuffer set
	// vertices of every object
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride, // how many bytes are in each vertex
		&offset
		);
	// Indexbuffer set
	// indices of every object
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
	// Vertex Shader Set Constant Buffers, different for vertex and pixel shader
	// vertex shader - does all vertices in scene. can set different shaders to customize the looks of objects
	//				 - where the multiplication of matrices happen
	// pixel shader - works per pixel, all the vertices in the world go from 3D space and translated into flat image for screen
	context->VSSetConstantBuffers(
		1,
		1,
		m_constantBuffer_View.GetAddressOf()
		);
	context->VSSetConstantBuffers(
		2,
		1,
		m_constantBuffer_Proj.GetAddressOf()
		);
	context->VSSetConstantBuffers(
		3,
		1,
		m_constantBuffer_Model.GetAddressOf()
		);

	// And the pixel shader
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	// Send the same constant buffer to the pixel shader
	context->PSSetConstantBuffers(
		4,
		1,
		m_constantBuffer_Material.GetAddressOf()
		);
	context->PSSetConstantBuffers(
		5,
		1,
		m_constantBuffer_Light.GetAddressOf()
		);

	context->UpdateSubresource(
		m_constantBuffer_Light.Get(),
		0,
		NULL,
		&m_constantBufferData_Light,
		0,
		0
		);

	/* Shaders and buffers set. Begin draw calls */
	// 1st - draw static objects
	// 2nd - draw dynamic objects
	for (unsigned int x = 0; x < staticObject_IndexCount.size(); x++) {
		// First, set the model matrix to render the static object and update the constant buffer
		if (staticObject_Transforms.size() > 0) {
			memcpy(&m_constantBufferData_Model.model, &staticObject_Transforms[x], sizeof(XMFLOAT4X4));
		}

		// _Material set in ShaderStructures.h
		// send material to shader
		m_constantBufferData_Material.material = _Material();

		// send model matrix to shader
		// do matrix calculations in GPU
		context->UpdateSubresource(
			m_constantBuffer_Material.Get(),
			0,
			NULL,
			&m_constantBufferData_Material,
			0,
			0
			);
		context->UpdateSubresource(
			m_constantBuffer_Model.Get(),
			0,
			NULL,
			&m_constantBufferData_Model,
			0,
			0
			);

		// Draw the static object
		context->DrawIndexed(
			staticObject_IndexCount[x],
			staticObject_StartIndexOffset[x],
			staticObject_StartVertexOffset[x]
			);

		// Reset model matrix back to identity
		XMStoreFloat4x4(&m_constantBufferData_Model.model, XMMatrixTranspose(XMMatrixIdentity()));
	}
	
	// Loops through all dynamic objects
	// changed to only draw the number of objects relative to this life aka draw num_cubes = life_number
	//for (int x = 0; x < dynamicObject_IndexCount.size(); x++) {
	for (int x = 0; x < current_game_info.current_life + 1; x++) {

		// First, set the model matrix to render the static object and update the constant buffer
		// make sure actually a transform object in existence
		// dynamicObject_Transforms stores model matrix for all dynamic object
		// model matrix set size position and rotation of object
		if (dynamicObject_Transforms.size() > 0) {
			memcpy(&m_constantBufferData_Model.model, &dynamicObject_Transforms[x], sizeof(XMFLOAT4X4));
		}

		m_constantBufferData_Material.material = _Material();
	
		context->UpdateSubresource(
			m_constantBuffer_Material.Get(),
			0,
			NULL,
			&m_constantBufferData_Material,
			0,
			0
			);
		context->UpdateSubresource(
			m_constantBuffer_Model.Get(),
			0,
			NULL,
			&m_constantBufferData_Model,
			0,
			0
			);

		// Draw the dynamic object
		context->DrawIndexed(
			dynamicObject_IndexCount[x],		// number of indices this object has (e.g. triangle has 3 indices, square has 6)
			dynamicObject_StartIndexOffset[x],	// first position of this object located at this position in the big index array
			dynamicObject_StartVertexOffset[x]	// first position of this object located at this position in the big vertex array
			);

		// Reset model matrix back to identity
		XMStoreFloat4x4(&m_constantBufferData_Model.model, XMMatrixTranspose(XMMatrixIdentity()));
	}
	
}

/*  REALLY COMPLEX
	Do different things for PC, Tablet, Phone.
	Only called once, sets up resources at start.
*/
void SceneRenderer::CreateDeviceDependentResources()
{

	// Setup the keyboard/mouse controller
	m_controller = ref new MoveLookController();
	m_controller->Initialize(CoreWindow::GetForCurrentThread());

	// REMEMBER: Asynchronous means it continues in the background and doesn't block
	// Load shaders asynchronously on separate threads, so they can both be loaded at the same time
	// Note that these .cso files are generates from the .hlsl shader files on the right, they're like the
	// shader byte-code
	auto loadVSTask = DX::ReadDataAsync(L"VertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	// The ".then" syntax means "wait for X to finish doing its thing, then execute the following code"
	// ".then" makes an additional thread
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
		// XMFLOAT3 12 bytes long, use 0-11 as position value, 12-23 as normal, 24-35 as color
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
	}); // vertex shader completed loading

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
		CD3D11_BUFFER_DESC constantBufferDesc_Model(sizeof(ModelCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc_Model,
			nullptr,
			&m_constantBuffer_Model
			)
			);
		CD3D11_BUFFER_DESC constantBufferDesc_View(sizeof(ViewCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc_View,
			nullptr,
			&m_constantBuffer_View
			)
			);
		CD3D11_BUFFER_DESC constantBufferDesc_Proj(sizeof(ProjCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc_Proj,
			nullptr,
			&m_constantBuffer_Proj
			)
			);
		CD3D11_BUFFER_DESC constantBufferDesc_Material(sizeof(MaterialCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc_Material,
			nullptr,
			&m_constantBuffer_Material
			)
			);
		CD3D11_BUFFER_DESC constantBufferDesc_Light(sizeof(LightCBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc_Light,
			nullptr,
			&m_constantBuffer_Light
			)
			);
	}); // pixel shader done loading

	// Once both shaders are loaded, create the mesh.
	// Notice how we are &&ing the two tasks, which means we wait for both to completely finish, then we do...
	auto createLandscapeTask = (createPSTask && createVSTask).then([this]() {
		// make it same as bitmap
		unsigned short landscapeSize = 96;
		Landscape landscape(landscapeSize, landscapeSize, &wallList); // also sets up wall collisions
		Water water(landscapeSize, landscapeSize);

		// this is our cube
		MoveObject moveObject(0.5f,0.5f,0.5f);

		// This creates the data (vertices) to put into the vertex buffer, and zeroes it
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		std::vector<VertexPositionNormalColour> vertices; // stores every single objects vertices, then load it into GPU

		// total number of vertices for allocating memory
		unsigned int vertexNumber = landscape.getVertexCount() + moveObject.getVertexCount() + water.getVertexCount();

		// put all our vertices together
		vertices.reserve(vertexNumber); // preallocate memory
		vertices.insert(vertices.end(), landscape.vertices.begin(), landscape.vertices.end());
		vertices.insert(vertices.end(), water.vertices.begin(), water.vertices.end());
		vertices.insert(vertices.end(), moveObject.vertices.begin(), moveObject.vertices.end());

		// the data for vertexBufferData is in the vertices array
		// pSysMem is a pointer to the data to put in
		vertexBufferData.pSysMem = &(vertices[0]);
		// This is only used for textures, so make it 0 as we arent using textures here
		vertexBufferData.SysMemPitch = 0;
		// Ditto for this
		vertexBufferData.SysMemSlicePitch = 0;

		// Here we actually create the vertex buffer. It's length is the size of the vertex array
		// Note this isn't a function, it's an initialiser for a variable called vertexBufferDesc

		int vertexCount = (vertexNumber)*sizeof(VertexPositionNormalColour);
		CD3D11_BUFFER_DESC vertexBufferDesc(vertexCount, D3D11_BIND_VERTEX_BUFFER);
		// And here we send it to the device like we did with the shaders
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData, // big list of verices
			&m_vertexBuffer    // create buffer, and store that buffer in m_vertexBuffer
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

		XMFLOAT4X4 tempMatrix;
		XMFLOAT4X4 objectPosition;
		XMStoreFloat4x4(&tempMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&objectPosition, XMMatrixIdentity());

		objectPosition._14 = -28;
		objectPosition._24 = 4;
		objectPosition._34 = -28;

		int currentVertexCount = 0; // Counter for current length of vertex array

		// STATIC OBJECTS

		// Insert the landscape
		staticObject_StartIndexOffset.push_back(indices.size());
		staticObject_StartVertexOffset.push_back(currentVertexCount);
		staticObject_IndexCount.push_back(landscape.getIndexCount());
		// insert at next available position, all indices from begin-end for landscape
		indices.insert(indices.end(), landscape.indices.begin(), landscape.indices.end());
		// sets initial position
		staticObject_Transforms.push_back(tempMatrix);
		currentVertexCount += landscape.getVertexCount();

		// Insert the water
		staticObject_StartIndexOffset.push_back(indices.size());
		staticObject_StartVertexOffset.push_back(currentVertexCount);
		staticObject_IndexCount.push_back(water.getIndexCount());
		indices.insert(indices.end(), water.indices.begin(), water.indices.end());
		staticObject_Transforms.push_back(tempMatrix);
		currentVertexCount += water.getVertexCount();

		// DYNAMIC OBJECTS

		// Insert the moving object
		dynamicObject_StartIndexOffset.push_back(indices.size());
		dynamicObject_StartVertexOffset.push_back(currentVertexCount);
		dynamicObject_IndexCount.push_back(moveObject.getIndexCount());
		indices.insert(indices.end(), moveObject.indices.begin(), moveObject.indices.end());
		dynamicObject_Transforms.push_back(objectPosition);
		currentVertexCount += moveObject.getVertexCount();
		
		// Make all the other cubes
		MakeCubes();

		////////////////////////
		/* MODEL INDICES DONE */
		////////////////////////


		// Store the length of the index array
		m_indexCount = landscape.getIndexCount() + moveObject.getIndexCount()  + water.getIndexCount();

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

		// Ambient light colour is always constant, so update it once
		m_constantBufferData_Light.GlobalAmbientColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
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
		fovAngleY, // vertical FOV
		aspectRatio,
		0.01f, // near clipping plane
		100.0f // far clipping plane
		);

	// Orientation is the orientation of a monitor, ie landscape or portrait
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	// The orientation matrix automatically transforms the perspective according to the orientation.
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	// This combines the projection and orientation matrices into the final projection matrix.
	XMStoreFloat4x4(
		&m_constantBufferData_Proj.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	//By the end of this method, we have the view and proj matrices saved into the constant buffer
	//Just need the model/world matrix and we are good to go (see Rotate() for this)
}

// takes in the number of cubes to create
// this INCLUDES the very first cube
// num_cubes = number of lives allowed, 1 cube per life
void SceneRenderer::MakeCubes(){
	int num_cubes = current_game_info.max_lives;

	if (num_cubes <= 1) {
		return; // don't add any cubes
	}
	else { // add 1 or more cubes
		// make the cubes
		// we know that the first cube is stored in dynamicObject_Transforms[0]
		for (int x = 0; x < num_cubes; x++) {
			dynamicObject_Transforms.push_back(dynamicObject_Transforms[0]);
			dynamicObject_IndexCount.push_back(dynamicObject_IndexCount[0]);
			dynamicObject_StartIndexOffset.push_back(dynamicObject_StartIndexOffset[0]);
			dynamicObject_StartVertexOffset.push_back(dynamicObject_StartVertexOffset[0]);
		}
	}
}
