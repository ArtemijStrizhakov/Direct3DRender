#pragma once

#include <functional>
#include "PixelShader.h"
#include "VertexShader.h"
#include "Buffer.h"

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class CRenderContext
{
public:
	CRenderContext();
	virtual ~CRenderContext();

	BOOL Initialize(HWND hWnd, int nWidth, int nHeigth);
	void ShutDown();
	void Render(std::function<void (ID3D11DeviceContext*)> render);
	
	
	CPixelShader::Ptr CreatePixelShader(std::wstring const & shaderPath);
	CVertexShader::Ptr CreateVertexShader(std::wstring const & shaderPath);
	
	template <class ITEM>
	typename CBuffer<ITEM>::Ptr CreateBuffer(BufferType type)
	{
		return std::make_shared<CBuffer<ITEM>>(m_pDevice, type);
	}

private:

	HWND m_hWnd;

	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
};

