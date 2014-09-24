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
		OutputDebugString(L"Here0\n");
		Direct3DBase::CreateDeviceResources();

		auto loadVSTask = DX::ReadDataAsync("VertexShader.cso");
		auto loadPSTask = DX::ReadDataAsync("PixelShader.cso");
		OutputDebugString(L"RRIGHT HEREOP0.1\n");
		auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
			DX::ThrowIfFailed(
				m_d3dDevice->CreateVertexShader(
 					fileData->Data,
					fileData->Length,
					nullptr,
					&m_vertexShader
					)
				);
			OutputDebugString(L"HereErreeOP0.1\n");
			const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			OutputDebugString(L"Here0.1\n");
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

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					&m_constantBuffer
					)
				);
			OutputDebugString(L"Here0.2\n");
		});

		auto createLandscapeTask = (createPSTask && createVSTask).then([this] () {
			OutputDebugString(L"Here1\n");
			Landscape landscape(6, 2);

			/*VertexPositionNormalColour vs[] = {
				VertexPositionNormalColour(XMFLOAT3(-0.5f,0,-0.5f), XMFLOAT3(0,1,0),XMFLOAT3(1,0,0)),
				VertexPositionNormalColour(XMFLOAT3(0.5f,0,-0.5f), XMFLOAT3(0,1,0),XMFLOAT3(0,1,0)),
				VertexPositionNormalColour(XMFLOAT3(-0.5f,0,0.5f), XMFLOAT3(0,1,0),XMFLOAT3(0,0,1)),
				VertexPositionNormalColour(XMFLOAT3(0.5f,0,0.5f), XMFLOAT3(0,1,0),XMFLOAT3(1,1,1)),
			};*/

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

			//unsigned short is[] = { 0,2,3,0,3,1 };

			m_indexCount = landscape.getIndexCount();
			OutputDebugString(L"Here2\n");
			D3D11_SUBRESOURCE_DATA indexBufferData = {0};
			indexBufferData.pSysMem = landscape.indices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			OutputDebugString(L"Here2.1\n");
			CD3D11_BUFFER_DESC indexBufferDesc(m_indexCount*sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
			OutputDebugString(L"Here2.1\n");
			DX::ThrowIfFailed(
				m_d3dDevice->CreateBuffer(
					&indexBufferDesc,
					&indexBufferData,
					&m_indexBuffer
					)
				);
			OutputDebugString(L"Here3\n");
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
		(void) timeDelta; // Unused parameter.

		XMVECTOR eye = XMVectorSet(0.0f, 3.0, 3.0f, 0.0f);
		XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));//RotationY(timeTotal * XM_PIDIV4)));
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