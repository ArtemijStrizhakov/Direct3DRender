#include "stdafx.h"
#include "IsoSurfaceRender.h"
#include "MarchingCubes.h"


CIsoSurfaceRender::CIsoSurfaceRender()
{

}


CIsoSurfaceRender::~CIsoSurfaceRender()
{

}


void CIsoSurfaceRender::CreateSampligGrid( int nCells, CBuffer<ISO_VERTEX>::Ptr spVertexBuffer )
{
	double step = 2.0f/nCells;
	
	double xPos = -1.0;

	for(int x = 0; x < nCells; x++)
	{
		double yPos = -1.0;

		for(int y = 0; y < nCells; y++)
		{
			double zPos = -1.0;

			for(int z = 0; z < nCells; z++)
			{
				spVertexBuffer->m_Items.push_back(ISO_VERTEX(XMFLOAT4(xPos, yPos, zPos, 1)));
					
				zPos += step;
			}

			yPos += step;
		}

		xPos += step;
	}

	spVertexBuffer->Compile();

}


bool CIsoSurfaceRender::Initialize( HWND hWnd, int nWidth, int nHeigth )
{
	m_RenderContext.Initialize(hWnd, nWidth, nHeigth);

	m_spPixelSader = m_RenderContext.CreatePixelShader(L"IsoSurface.fx");

	m_spVertexShader = m_RenderContext.CreateVertexShader(L"IsoSurface.fx");

	m_spGeometryShader = m_RenderContext.CreateGeometryShader(L"IsoSurface.fx");

	//////////////////////////////////////////////////////////////////////////

	m_spVertexBuffer = m_RenderContext.CreateBuffer<ISO_VERTEX>(VertexBuffer);

	CreateSampligGrid(20, m_spVertexBuffer);

	m_spConstantBuffer = m_RenderContext.CreateBuffer<ISO_CONSTANTBUFER>(ConstantBuffer);

	m_World1 = XMMatrixIdentity();

	m_Constants.world = m_World1;
	m_Constants.view = XMMatrixLookAtLH( XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f ), XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ), XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
	m_Constants.projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, nWidth / (FLOAT)nHeigth, 0.01f, 100.0f );

	m_spConstantBuffer->m_Items.push_back(m_Constants);

	m_spConstantBuffer->Compile();

	return true;
}

void CIsoSurfaceRender::ShutDown()
{
	m_spVertexBuffer.reset();
	m_spGeometryShader.reset();
	m_spPixelSader.reset();
	m_spVertexShader.reset();

	m_RenderContext.ShutDown();
}

void CIsoSurfaceRender::Render()
{
	static float t = 0.0f;
	t += ( float )XM_PI * 0.0001f;

	m_World1 = XMMatrixRotationY(sin(t)*0.5)*XMMatrixRotationX(0)*XMMatrixScaling(3,3,3);

	m_RenderContext.Render([this](CRenderContext* pContext)
	{
		pContext->SetVertexBuffer(m_spVertexBuffer->GetBuffer(), sizeof( ISO_VERTEX ));

		pContext->SetVertexShaderConstantBuffer(m_spConstantBuffer->GetBuffer());
		pContext->SetGeometryShaderConstantBuffer(m_spConstantBuffer->GetBuffer());

		pContext->SetPixelShader(m_spPixelSader->GetShader());

		pContext->SetVertexShader(m_spVertexShader->GetShader(), m_spVertexShader->GetLayout());

		pContext->SetGeometryShader(m_spGeometryShader->GetShader());		

		pContext->GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

		m_Constants.world = m_World1;
		m_spConstantBuffer->m_Items[0] = m_Constants.PrepareForBuffer();
		m_spConstantBuffer->Update();
		pContext->GetDeviceContext()->Draw(m_spVertexBuffer->m_Items.size(), 0);

	});
}

