#pragma once

#include "RenderContext.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include <xnamath.h>

struct ISO_VERTEX
{
	ISO_VERTEX()
	{
		position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}


	ISO_VERTEX(XMFLOAT4 p)
		: position(p)

	{

	}

	XMFLOAT4 position;
};

struct ISO_CONSTANTBUFER
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	ISO_CONSTANTBUFER PrepareForBuffer()
	{
		ISO_CONSTANTBUFER transposed;

		transposed.world = XMMatrixTranspose( world );
		transposed.view = XMMatrixTranspose( view );
		transposed.projection = XMMatrixTranspose( projection );

		return transposed;
	}
};

class CIsoSurfaceRender
{
public:
	CIsoSurfaceRender();
	~CIsoSurfaceRender();
	
	bool Initialize(HWND hWnd, int nWidth, int nHeigth);
	void ShutDown();
	void Render();

protected:

	void CreateSampligGrid(int nCells, CBuffer<ISO_VERTEX>::Ptr spVertexBuffer);

	CRenderContext m_RenderContext;

	CPixelShader::Ptr m_spPixelSader;
	CVertexShader::Ptr m_spVertexShader;
	CGeometryShader::Ptr m_spGeometryShader;

	CBuffer<ISO_VERTEX>::Ptr m_spVertexBuffer;
	CBuffer<ISO_CONSTANTBUFER>::Ptr m_spConstantBuffer;

	ISO_CONSTANTBUFER m_Constants;
	XMMATRIX m_World1;
};

