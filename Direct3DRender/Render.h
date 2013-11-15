#pragma once
#include "RenderContext.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include <xnamath.h>

struct VERTEX
{
	VERTEX(XMFLOAT4 p, XMFLOAT4 c)
		: position(p)
		, color(c)
	{

	}

	XMFLOAT4 position;
	XMFLOAT4 color;
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


class CRender
{
public:
	CRender();
	virtual ~CRender();

	bool Initialize(HWND hWnd, int nWidth, int nHeigth);
	void ShutDown();
	void Render();

protected:

	CRenderContext m_RC;
	CPixelShader::Ptr m_PS;
	CVertexShader::Ptr m_VS;
	CBuffer<VERTEX>::Ptr m_VB;
	CBuffer<WORD>::Ptr m_IB;
	CBuffer<CONSTANTBUFER>::Ptr m_CB;

	CONSTANTBUFER m_Constants;

};

