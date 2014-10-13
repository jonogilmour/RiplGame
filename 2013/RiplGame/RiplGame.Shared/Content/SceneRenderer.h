#pragma once

#include "pch.h"
#include "Content\Structures\ShaderStructures.h"
#include "Content\MoveLookController.h"

using namespace RiplGame;

namespace RiplGame
{
	// This sample renderer instantiates a basic rendering pipeline.
	class SceneRenderer
	{
	public:
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		SceneRenderer();

		// Vectors for storing draw-related values for static and dynamic objects
		std::vector<uint32> staticObject_StartIndexOffset;
		std::vector<uint32> dynamicObject_StartIndexOffset;
		std::vector<uint32> staticObject_StartVertexOffset;
		std::vector<uint32> dynamicObject_StartVertexOffset;
		std::vector<uint32> staticObject_IndexCount;
		std::vector<uint32> dynamicObject_IndexCount;

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		bool m_loadingComplete;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

		uint32 m_indexCount;
		ShaderCBuffer m_constantBufferData;
		MoveLookController^ m_controller;

		
	};
}

