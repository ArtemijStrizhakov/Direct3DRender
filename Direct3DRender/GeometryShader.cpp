#include "stdafx.h"
#include "GeometryShader.h"
#include "ShaderUtils.h"
#include "DXErrors.h"

#include <string>
#include <d3dcommon.h>
#include <d3d11.h>

CGeometryShader::CGeometryShader()
	: m_pGeometryShader(nullptr)
{

}


CGeometryShader::~CGeometryShader()
{
	if (m_pGeometryShader != nullptr)
	{
		m_pGeometryShader->Release();
		m_pGeometryShader = nullptr;
	}
}

bool CGeometryShader::Compile( std::wstring const & shaderPath, ID3D11Device* pDevice )
{
	ID3DBlob* pPSBlob = nullptr;

	HRESULT hr = ShaderUtils::CompileShaderFromFile(shaderPath, "GS", "gs_4_0", &pPSBlob );

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	hr = pDevice->CreateGeometryShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pGeometryShader );
	pPSBlob->Release();

	if(FAILED(hr))
	{
		DXErrors::ReportError(hr);
		return false;
	}

	return true;
}

