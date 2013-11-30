#include "stdafx.h"
#include "Render.h"
#include <xnamath.h>


CRender::CRender()
{

}


CRender::~CRender()
{

}

void CRender::GenerateGrid( int nXCount, int nYCount, std::vector<VERTEX>& vertices, std::vector<WORD>& triangleIndexes )
{
	float startX = -1.0f;
	float startY = -1.0f;

	float stepX = 2.0f/nXCount;
	float stepY = 2.0f/nYCount;

	float currentY = startY;

	for(int y = 0; y <= nYCount; y++)
	{

		float currentX = startX;

		for(int x = 0; x <= nXCount; x++)
		{
			VERTEX v;

			v.position.x = currentX;
			v.position.y = currentY;
			v.position.z =  sin(currentX*currentY*6)*0.2;
			v.position.w = 1.0f;

			vertices.push_back(v);

			currentX += stepX;
		}

		currentY += stepY;
	}



	for(int y = 0; y < nYCount; y++)
	{
		for(int x = 0; x <  nXCount; x++)
		{
			triangleIndexes.push_back(x + y*(nXCount + 1) + 1);
			triangleIndexes.push_back(x + y*(nXCount + 1));
			triangleIndexes.push_back(x + (y + 1)*(nXCount + 1));

			triangleIndexes.push_back(x + y*(nXCount + 1) + 1);
			triangleIndexes.push_back(x + (y + 1)*(nXCount + 1));
			triangleIndexes.push_back(x + (y + 1)*(nXCount + 1) + 1);


		}
	}


}

bool CRender::Initialize( HWND hWnd, int nWidth, int nHeigth )
{
	m_RenderContext.Initialize(hWnd, nWidth, nHeigth);
	
	m_spPixelSader = m_RenderContext.CreatePixelShader(L"shaders.fx");
	
	m_spVertexShader = m_RenderContext.CreateVertexShader(L"shaders.fx");

	m_spGeometryShader = m_RenderContext.CreateGeometryShader(L"shaders.fx");

	//////////////////////////////////////////////////////////////////////////

	m_spVertexBuffer = m_RenderContext.CreateBuffer<VERTEX>(VertexBuffer);
		
/*	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, 1.0f, -1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, 1.0f, -1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, 1.0f, 1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, -1.0f, -1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, -1.0f, -1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, -1.0f, 1.0f, 1.0f ) ) );
	m_spVertexBuffer->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, -1.0f, 1.0f, 1.0f ) ) );
	m_spVertexBuffer->Compile();
*/
	//////////////////////////////////////////////////////////////////////////

	m_spIndexBuffer = m_RenderContext.CreateBuffer<WORD>(IndexBuffer);

/*	WORD indexes[] = {3,1,0, 2,1,3,	0,5,4, 1,5,0, 3,4,7, 0,4,3, 1,6,5, 2,6,1, 2,7,6, 3,7,2, 6,4,5, 7,4,6,};

	std::vector<WORD> v(indexes, indexes + sizeof(indexes) / sizeof(WORD));

	m_spIndexBuffer->m_Items = v;

	m_spIndexBuffer->Compile();
*/
	//////////////////////////////////////////////////////////////////////////

	GenerateGrid(200, 200, m_spVertexBuffer->m_Items, m_spIndexBuffer->m_Items);

	m_spVertexBuffer->Compile();
	m_spIndexBuffer->Compile();

	m_spConstantBuffer = m_RenderContext.CreateBuffer<CONSTANTBUFER>(ConstantBuffer);

	m_World1 = XMMatrixIdentity();

	m_Constants.world = m_World1;
	m_Constants.view = XMMatrixLookAtLH( XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f ), XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ), XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
	m_Constants.projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, nWidth / (FLOAT)nHeigth, 0.01f, 100.0f );
		
	m_spConstantBuffer->m_Items.push_back(m_Constants);
	
	m_spConstantBuffer->Compile();

	return true;
}

void CRender::ShutDown()
{
	m_spVertexBuffer.reset();
	m_spGeometryShader.reset();
	m_spPixelSader.reset();
	m_spVertexShader.reset();

	m_RenderContext.ShutDown();
}

void CRender::Render()
{
	static float t = 0.0f;
	t += ( float )XM_PI * 0.0001f;

	m_World1 = XMMatrixRotationY(sin(t))*XMMatrixScaling(3,3,3);
		
	m_RenderContext.Render([this](CRenderContext* pContext)
		{
			pContext->SetVertexBuffer(m_spVertexBuffer->GetBuffer(), sizeof( VERTEX ));
		
			pContext->SetIndexBuffer(m_spIndexBuffer->GetBuffer());
			
			pContext->SetConstantBuffer(m_spConstantBuffer->GetBuffer());

			pContext->SetPixelShader(m_spPixelSader->GetShader());
			
			pContext->SetVertexShader(m_spVertexShader->GetShader(), m_spVertexShader->GetLayout());

			pContext->SetGeometryShader(m_spGeometryShader->GetShader());		
			
			pContext->GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

			m_Constants.world = m_World1;
			m_spConstantBuffer->m_Items[0] = m_Constants.PrepareForBuffer();
			m_spConstantBuffer->Update();
			pContext->GetDeviceContext()->DrawIndexed( m_spIndexBuffer->m_Items.size(), 0, 0 );
				
		});
}

