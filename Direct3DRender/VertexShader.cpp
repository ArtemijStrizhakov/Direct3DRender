#include "stdafx.h"
#include "VertexShader.h"
#include "ShaderUtils.h"
#include "DXErrors.h"

#include <string>
#include <d3dcommon.h>
#include <d3d11.h>

CVertexShader::CVertexShader()
: m_pVertexShader(nullptr)
, m_pVertexLayout(nullptr)
{

}


CVertexShader::~CVertexShader()
{
	if (m_pVertexLayout != nullptr)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}

	if (m_pVertexShader != nullptr)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}
}

bool CVertexShader::Compile( std::wstring const & shaderPath, ID3D11Device* pDevice )
{
	ID3DBlob* pVSBlob = nullptr;

	HRESULT hr = ShaderUtils::CompileShaderFromFile(shaderPath, "VS", "vs_4_0", &pVSBlob );

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	hr = pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
		
	if(FAILED(hr))
	{
		pVSBlob->Release();
		DXErrors::ReportError(hr);
		return false;
	}


	ShaderUtils::CreateInputLayoutDescFromVertexShaderSignature(pVSBlob, pDevice, &m_pVertexLayout);

	pVSBlob->Release();
	
	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	return true;
}
