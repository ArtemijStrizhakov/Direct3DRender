#include "stdafx.h"
#include "ShaderUtils.h"

#include <d3dcompiler.h>
#include <D3DX11Async.h>
#include <vector>

namespace ShaderUtils
{
	HRESULT CompileShaderFromFile( std::wstring const & strFileName, std::string const &  strEntryPoint, std::string const &  strShaderModel, ID3DBlob** ppBlobOut )
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile( strFileName.c_str(), NULL, NULL, strEntryPoint.c_str(), strShaderModel.c_str(), 
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
		if( FAILED(hr) )
		{
			if( pErrorBlob != NULL )
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			return hr;
		}
		if( pErrorBlob ) pErrorBlob->Release();

		return S_OK;
	}

	HRESULT CreateInputLayoutDescFromVertexShaderSignature( ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout )
	{
		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	
		HRESULT hr = D3DReflect( pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection );

		if ( FAILED( hr ) )
		{
			return hr;
		}

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc( &shaderDesc );

		// Read input layout description from shader info
		int byteOffset = 0;

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		
		for ( int i=0; i< shaderDesc.InputParameters; i++ )
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;      
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc );

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;  
			elementDesc.SemanticName = paramDesc.SemanticName;     
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = byteOffset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;  

			// determine DXGI format
			if ( paramDesc.Mask == 1 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32_UINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32_SINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				}

				byteOffset += 4;
			}
			else if ( paramDesc.Mask <= 3 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				}

				byteOffset += 8;
			}
			else if ( paramDesc.Mask <= 7 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}

				byteOffset += 12;
			}
			else if ( paramDesc.Mask <= 15 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				}
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}

				byteOffset += 16;
			}

			inputLayoutDesc.push_back(elementDesc);
		}      

		hr = pD3DDevice->CreateInputLayout( inputLayoutDesc.data(), inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout );

		pVertexShaderReflection->Release();
		
		return hr;
	}

};
