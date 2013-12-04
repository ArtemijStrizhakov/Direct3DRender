#pragma once
#include "RenderContext.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include <xnamath.h>

struct VERTEX
{
	VERTEX()
	{
		position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}


	VERTEX(XMFLOAT4 p)
		: position(p)
		
	{

	}
	
	XMFLOAT4 position;
};

struct CONSTANTBUFER
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	CONSTANTBUFER PrepareForBuffer()
	{
		CONSTANTBUFER transposed;

		transposed.world = XMMatrixTranspose( world );
		transposed.view = XMMatrixTranspose( view );
		transposed.projection = XMMatrixTranspose( projection );

		return transposed;
	}
};


class CFractalRender
{
public:
	CFractalRender();
	virtual ~CFractalRender();

	bool Initialize(HWND hWnd, int nWidth, int nHeigth);
	void ShutDown();
	void Render();

protected:

	void GenerateGrid(int nXCount, int nYCount, std::vector<VERTEX>& vertices, std::vector<WORD>& triangleIndexes);
	
	CRenderContext m_RenderContext;
	
	CPixelShader::Ptr m_spPixelSader;
	CVertexShader::Ptr m_spVertexShader;
	CGeometryShader::Ptr m_spGeometryShader;
	
	CBuffer<VERTEX>::Ptr m_spVertexBuffer;
	CBuffer<WORD>::Ptr m_spIndexBuffer;
	CBuffer<CONSTANTBUFER>::Ptr m_spConstantBuffer;

	CONSTANTBUFER m_Constants;
	XMMATRIX m_World1;

};

