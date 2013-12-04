#include "stdafx.h"
#include "IsoSurfaceRender.h"
#include "MarchingCubes.h"


CIsoSurfaceRender::CIsoSurfaceRender()
{

}


CIsoSurfaceRender::~CIsoSurfaceRender()
{

}


void CIsoSurfaceRender::PoligonizeField( int nCells, CBuffer<ISO_VERTEX>::Ptr spVertexBuffer )
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
				MarchingCubes::GRIDCELL gc;

				gc.p[0].x = xPos;
				gc.p[0].y = yPos;
				gc.p[0].z = zPos;

				gc.p[1].x = xPos + step;
				gc.p[1].y = yPos;
				gc.p[1].z = zPos;

				gc.p[2].x = xPos + step;
				gc.p[2].y = yPos;
				gc.p[2].z = zPos + step;

				gc.p[3].x = xPos;
				gc.p[3].y = yPos;
				gc.p[3].z = zPos + step;

				gc.p[4].x = xPos;
				gc.p[4].y = yPos + step;
				gc.p[4].z = zPos;

				gc.p[5].x = xPos + step;
				gc.p[5].y = yPos + step;
				gc.p[5].z = zPos;

				gc.p[6].x = xPos + step;
				gc.p[6].y = yPos + step;
				gc.p[6].z = zPos + step;

				gc.p[7].x = xPos;
				gc.p[7].y = yPos + step;
				gc.p[7].z = zPos + step;

				for(int i = 0; i < 8; i++)
				{
					gc.val[i] = 1/sqrt((gc.p[i].x + 1)*(gc.p[i].x + 1) + gc.p[i].y*gc.p[i].y + gc.p[i].z*gc.p[i].z);
				}

				for(int i = 0; i < 8; i++)
				{
					gc.val[i] += 1/sqrt((gc.p[i].x - 1)*(gc.p[i].x - 1) + gc.p[i].y*gc.p[i].y + gc.p[i].z*gc.p[i].z);
				}
			

				MarchingCubes::Polygonise(gc, 1.9, [&spVertexBuffer](double x, double y, double z)
					{
						spVertexBuffer->m_Items.push_back(ISO_VERTEX(XMFLOAT4(x, y, z, 1)));
					});

				
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

		PoligonizeField(20, m_spVertexBuffer);

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


	m_World1 = XMMatrixRotationY(sin(t)*0.5)/**XMMatrixRotationX(1)*/*XMMatrixScaling(3,3,3);

	m_RenderContext.Render([this](CRenderContext* pContext)
	{
		pContext->SetVertexBuffer(m_spVertexBuffer->GetBuffer(), sizeof( ISO_VERTEX ));

		pContext->SetConstantBuffer(m_spConstantBuffer->GetBuffer());

		pContext->SetPixelShader(m_spPixelSader->GetShader());

		pContext->SetVertexShader(m_spVertexShader->GetShader(), m_spVertexShader->GetLayout());

		pContext->SetGeometryShader(m_spGeometryShader->GetShader());		

		pContext->GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		m_Constants.world = m_World1;
		m_spConstantBuffer->m_Items[0] = m_Constants.PrepareForBuffer();
		m_spConstantBuffer->Update();
		pContext->GetDeviceContext()->Draw(m_spVertexBuffer->m_Items.size(), 0);

	});
}

