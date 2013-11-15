#pragma once

#include <vector>
#include <memory>
#include <d3d11.h>
#include "DXErrors.h"

enum BufferType
{
	VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
	IndexBuffer = D3D11_BIND_INDEX_BUFFER,
	ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
};


template <class ITEM>
class CBuffer
{
public:

	typedef std::shared_ptr<CBuffer> Ptr;

	CBuffer(ID3D11Device* pDevice, BufferType type)
	: m_pBuffer(nullptr)
	, m_pDevice(pDevice)
	, m_Type(type)
	{

	}


	~CBuffer()
	{
		if(m_pBuffer != nullptr)
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}
	}

	bool Compile()
	{
		if(m_pBuffer != nullptr)
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}

		D3D11_BUFFER_DESC bd = {0};
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( ITEM ) * m_Items.size();
		bd.BindFlags = (UINT)m_Type;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = (const void *) m_Items.data();

		HRESULT hr = m_pDevice->CreateBuffer( &bd, &InitData, &m_pBuffer );
		
		if( FAILED( hr ) )
		{
			DXErrors::ReportError(hr);
			return false;
		}

		return true;
	}

	ID3D11Buffer* GetBuffer()
	{
		return m_pBuffer;
	}

public:

	std::vector<ITEM> m_Items;
	
protected:

	ID3D11Device*	m_pDevice;
	ID3D11Buffer*	m_pBuffer;
	BufferType		m_Type;
};

