#pragma once

#include "Direct3DBase.h"
#include "pch.h"

namespace Ripl {
	ref class SceneRenderer sealed : public Direct3DBase
	{
	public:
		SceneRenderer();

		// Direct3DBase methods.
		virtual void CreateDeviceResources() override;
		virtual void CreateWindowSizeDependentResources() override;
		virtual void Render() override;
	
		// Method for updating time-dependent objects.
		void Update(float timeTotal, float timeDelta);

	private:
		bool m_loadingComplete;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

		MoveLookController^ m_controller;

		uint32 m_indexCount;
		ShaderCBuffer m_constantBufferData;
	};
}