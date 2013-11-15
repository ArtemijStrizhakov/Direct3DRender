#pragma once

#include <d3dcommon.h>
#include <string>
#include <d3d11.h>

namespace ShaderUtils
{
	HRESULT CompileShaderFromFile( std::wstring const & strFileName, std::string const &  strEntryPoint, std::string const &  strShaderModel, ID3DBlob** ppBlobOut );
	HRESULT CreateInputLayoutDescFromVertexShaderSignature( ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout );

};

