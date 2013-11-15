#include "stdafx.h"
#include "PixelShader.h"
#include "ShaderUtils.h"
#include "DXErrors.h"

#include <string>
#include <d3dcommon.h>
#include <d3d11.h>

CPixelShader::CPixelShader()
: m_pPixelShader(nullptr)
{

}


CPixelShader::~CPixelShader()
{
	if (m_pPixelShader != nullptr)
	{
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}
}

bool CPixelShader::Compile( std::wstring const & shaderPath, ID3D11Device* pDevice )
{
	ID3DBlob* pPSBlob = nullptr;

	HRESULT hr = ShaderUtils::CompileShaderFromFile(shaderPath, "PS", "ps_4_0", &pPSBlob );

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	hr = pDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
	pPSBlob->Release();

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	return true;
}
