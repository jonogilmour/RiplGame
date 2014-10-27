#pragma once

#include "pch.h"
#include "Content\Structures\ShaderStructures.h"
#include "Content\MoveLookController.h"
#include "Content\Structures\OtherStructures.h"


using namespace RiplGame;

namespace RiplGame
{
	// This sample renderer instantiates a basic rendering pipeline.
	class SceneRenderer
	{
	public:

		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void MakeCubes();
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
		std::vector<XMFLOAT4X4> staticObject_Transforms;
		std::vector<XMFLOAT4X4> dynamicObject_Transforms;
		static int diffVal;
		bool GameStarted;

	private:


		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		bool m_loadingComplete;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer_Model;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer_View;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer_Proj;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer_Material;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer_Light;

		uint32 m_indexCount; // total size of index buffer
		ModelCBuffer m_constantBufferData_Model;
		ViewCBuffer m_constantBufferData_View;
		ProjCBuffer m_constantBufferData_Proj;
		MaterialCBuffer m_constantBufferData_Material;
		LightCBuffer m_constantBufferData_Light;
		MoveLookController^ m_controller;

		std::list<XMFLOAT3> wallList;

		bool gameWon;
		bool gameEnded;
		int succeeds;
	};
}

