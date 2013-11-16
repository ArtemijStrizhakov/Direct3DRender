#include "stdafx.h"
#include "RenderContext.h"
#include "DXErrors.h"

#include <d3d11.h>



CRenderContext::CRenderContext()
: m_hWnd(nullptr)
, m_pSwapChain(nullptr)
, m_pDevice(nullptr)
, m_pImmediateContext(nullptr)
, m_pRenderTargetView(nullptr)
{

	
	std::shared_ptr<int> ptr;
}


CRenderContext::~CRenderContext()
{

}

BOOL CRenderContext::Initialize(HWND hWnd, int nWidth, int nHeigth)
{
	m_hWnd = hWnd;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = nWidth;
	sd.BufferDesc.Height = nHeigth;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(	NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL,
		0, 
		NULL, 
		0,
		D3D11_SDK_VERSION, 
		&sd, 
		&m_pSwapChain, 
		&m_pDevice, 
		NULL, 
		&m_pImmediateContext );

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		ShutDown();
		return FALSE;
	}

	ID3D11Texture2D* pBackBuffer = nullptr;

	hr = m_pSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer); 

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		ShutDown();
		return FALSE;
	}

	hr = m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );

	pBackBuffer->Release();
	pBackBuffer = nullptr;

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		ShutDown();
		return FALSE;
	}

	m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)nWidth;
	vp.Height = (FLOAT)nHeigth;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports( 1, &vp );

	return TRUE;
}

void CRenderContext::ShutDown()
{
	if(m_pImmediateContext != nullptr)
	{
		m_pImmediateContext->ClearState();
	}

	if(m_pRenderTargetView != nullptr) 
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}

	if(m_pSwapChain != nullptr)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}

	if(m_pImmediateContext != nullptr)
	{
		m_pImmediateContext->Release();
		m_pImmediateContext = nullptr;
	}

	if( m_pDevice != nullptr )
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}


}


void CRenderContext::Render(std::function<void (CRenderContext*)> render)
{
	if( m_pImmediateContext == nullptr ||
		m_pRenderTargetView == nullptr ||
		m_pSwapChain == nullptr )
	{
		return;
	}

	float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA

	m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

	render(this);

	m_pSwapChain->Present( 0, 0 );
}

CPixelShader::Ptr CRenderContext::CreatePixelShader(std::wstring const & shaderPath)
{
	auto shader = std::make_shared<CPixelShader>();

	if(!shader->Compile(shaderPath, m_pDevice))
	{
		return CPixelShader::Ptr();
	}

	return shader;
}

CVertexShader::Ptr CRenderContext::CreateVertexShader(std::wstring const & shaderPath)
{
	auto shader = std::make_shared<CVertexShader>();

	if(!shader->Compile(shaderPath, m_pDevice))
	{
		return CVertexShader::Ptr();
	}

	return shader;
}

void CRenderContext::SetVertexBuffer( ID3D11Buffer* pBuffer, int nSize )
{
	UINT stride = nSize;
	UINT offset = 0;

	std::vector<ID3D11Buffer*> buffers;
	buffers.push_back(pBuffer);
	ID3D11Buffer *const * ppBuffers = (ID3D11Buffer *const *)buffers.data(); 
	m_pImmediateContext->IASetVertexBuffers( 0, buffers.size(), ppBuffers, &stride, &offset );
}

void CRenderContext::SetIndexBuffer( ID3D11Buffer* pBuffer )
{
	m_pImmediateContext->IASetIndexBuffer( pBuffer, DXGI_FORMAT_R16_UINT, 0 );
}

void CRenderContext::SetConstantBuffer( ID3D11Buffer* pBuffer )
{
	std::vector<ID3D11Buffer*> buffers;
	buffers.push_back(pBuffer);
	ID3D11Buffer *const * ppBuffers = (ID3D11Buffer *const *)buffers.data(); 
	m_pImmediateContext->VSSetConstantBuffers( 0, 1, ppBuffers );
}

void CRenderContext::SetPixelShader( ID3D11PixelShader* pShader )
{
	m_pImmediateContext->PSSetShader( pShader, NULL, 0 );
}

void CRenderContext::SetVertexShader( ID3D11VertexShader* pShader, ID3D11InputLayout *pInputLayout )
{
	m_pImmediateContext->VSSetShader( pShader, NULL, 0 );
	m_pImmediateContext->IASetInputLayout( pInputLayout );

}

ID3D11DeviceContext* CRenderContext::GetDeviceContext()
{
	return m_pImmediateContext;
}
