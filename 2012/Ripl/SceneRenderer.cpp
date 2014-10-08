#include "pch.h"
#include "SceneRenderer.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

namespace Ripl {
	SceneRenderer::SceneRenderer() :
		m_loadingComplete(false),
		m_indexCount(0)
	{
	}

	void SceneRenderer::CreateDeviceResources()
	{
		Direct3DBase::CreateDeviceResources();

		// Setup the keyboard/mouse controller
		m_controller = ref new MoveLookController();
		m_controller->Initialize(CoreWindow::GetForCurrentThread());

		auto loadVSTask = DX::ReadDataAsync("VertexShader.cso");
		auto loadPSTask = DX::ReadDataAsync("PixelShader.cso");
		auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
			DX::ThrowIfFailed(
				m_d3dDevice->CreateVertexShader(
 					fileData->Data,
					fileData->Length,
					nullptr,
					&m_vertexShader
					)
				);
			const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			DX::ThrowIfFailed(
				m_d3dDevice->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					fileData->Data,
					fileData->Length,
					&m_inputLayout
					)
				);
		});

		auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
			DX::ThrowIfFailed(
				m_d3dDevice->CreatePixelShader(
					fileData->Data,
					fileData->Length,
					nullptr,
					&m_pixelShader
					)
				);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ShaderCBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					&m_constantBuffer
					)
				);
		});

		auto createLandscapeTask = (createPSTask && createVSTask).then([this] () {
			Landscape landscape(15, 15);

			D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
			vertexBufferData.pSysMem = &(landscape.vertices[0]);
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(landscape.getVertexCount()*sizeof(VertexPositionNormalColour), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateBuffer(
					&vertexBufferDesc,
					&vertexBufferData,
					&m_vertexBuffer
					)
				);

			m_indexCount = landscape.getIndexCount();
			D3D11_SUBRESOURCE_DATA indexBufferData = {0};
			indexBufferData.pSysMem = landscape.indices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(m_indexCount*sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateBuffer(
					&indexBufferDesc,
					&indexBufferData,
					&m_indexBuffer
					)
				);
		});

		createLandscapeTask.then([this] () {
			m_loadingComplete = true;
		});
	}

	void SceneRenderer::CreateWindowSizeDependentResources()
	{
		Direct3DBase::CreateWindowSizeDependentResources();

		float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
		float fovAngleY = 70.0f * XM_PI / 180.0f;

		// Note that the m_orientationTransform3D matrix is post-multiplied here
		// in order to correctly orient the scene to match the display orientation.
		// This post-multiplication step is required for any draw calls that are
		// made to the swap chain render target. For draw calls to other targets,
		// this transform should not be applied.
		XMStoreFloat4x4(
			&m_constantBufferData.projection,
			XMMatrixTranspose(
				XMMatrixMultiply(
					XMMatrixPerspectiveFovLH(
						fovAngleY,
						aspectRatio,
						0.01f,
						100.0f
						),
					XMLoadFloat4x4(&m_orientationTransform3D)
					)
				)
			);
	}

	void SceneRenderer::Update(float timeTotal, float timeDelta)
	{
		// Update all controller events
		m_controller->Update(CoreWindow::GetForCurrentThread(), timeDelta);

		XMVECTOR eye = XMLoadFloat3(&m_controller->get_Position());
		XMVECTOR at = XMLoadFloat3(&m_controller->get_LookAt());
		XMVECTOR up = XMLoadFloat3(&m_controller->get_UpAxis());

		// Setup the constant buffer
		m_constantBufferData.ambientColour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_constantBufferData.lightVector = XMFLOAT4(0, 1.0f, 0, 0);
		m_constantBufferData.lightColour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));
	}

	void SceneRenderer::Render()
	{
		const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
		m_d3dContext->ClearRenderTargetView(
			m_renderTargetView.Get(),
			midnightBlue
			);

		m_d3dContext->ClearDepthStencilView(
			m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH,
			1.0f,
			0
			);

		if (!m_loadingComplete)
		{
			return;
		}

		m_d3dContext->OMSetRenderTargets(
			1,
			m_renderTargetView.GetAddressOf(),
			m_depthStencilView.Get()
			);

		m_d3dContext->UpdateSubresource(
			m_constantBuffer.Get(),
			0,
			NULL,
			&m_constantBufferData,
			0,
			0
			);

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		m_d3dContext->IASetVertexBuffers(
			0,
			1,
			m_vertexBuffer.GetAddressOf(),
			&stride,
			&offset
			);

		m_d3dContext->IASetIndexBuffer(
			m_indexBuffer.Get(),
			DXGI_FORMAT_R16_UINT,
			0
			);

		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_d3dContext->IASetInputLayout(m_inputLayout.Get());

		m_d3dContext->VSSetShader(
			m_vertexShader.Get(),
			nullptr,
			0
			);

		m_d3dContext->VSSetConstantBuffers(
			0,
			1,
			m_constantBuffer.GetAddressOf()
			);

		m_d3dContext->PSSetShader(
			m_pixelShader.Get(),
			nullptr,
			0
			);

		m_d3dContext->DrawIndexed(
			m_indexCount,
			0,
			0
			);
	}
}