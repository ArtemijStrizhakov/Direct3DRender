#pragma once

#include <d3dcommon.h>
#include <string>

namespace ShaderUtils
{
	HRESULT CompileShaderFromFile( std::wstring const & strFileName, std::string const &  strEntryPoint, std::string const &  strShaderModel, ID3DBlob** ppBlobOut );
};

