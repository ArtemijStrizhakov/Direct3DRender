#pragma once
#include "RenderContext.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include <xnamath.h>

struct VERTEX
{
	VERTEX(XMFLOAT3 p, XMFLOAT4 c)
		: position(p)
		, color(c)
	{

	}

	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct CONSTANTBUFER
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
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

	CONSTANTBUFER cb;

};

