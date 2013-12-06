#include "stdafx.h"
#include "FractalRender.h"
#include <xnamath.h>


CFractalRender::CFractalRender()
{

}


CFractalRender::~CFractalRender()
{

}

void CFractalRender::GenerateGrid( int nXCount, int nYCount, std::vector<VERTEX>& vertices, std::vector<WORD>& triangleIndexes )
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
			v.position.z =  0.0;//sin(currentX*currentY*6)*0.1;
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

bool CFractalRender::Initialize( HWND hWnd, int nWidth, int nHeigth )
{
	m_RenderContext.Initialize(hWnd, nWidth, nHeigth);
	
	m_spPixelSader = m_RenderContext.CreatePixelShader(L"FractalRender.fx");
	
	m_spVertexShader = m_RenderContext.CreateVertexShader(L"FractalRender.fx");

	m_spGeometryShader = m_RenderContext.CreateGeometryShader(L"FractalRender.fx");

	//////////////////////////////////////////////////////////////////////////

	m_spVertexBuffer = m_RenderContext.CreateBuffer<VERTEX>(VertexBuffer);
		

	//////////////////////////////////////////////////////////////////////////

	m_spIndexBuffer = m_RenderContext.CreateBuffer<WORD>(IndexBuffer);

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

void CFractalRender::ShutDown()
{
	m_spVertexBuffer.reset();
	m_spGeometryShader.reset();
	m_spPixelSader.reset();
	m_spVertexShader.reset();

	m_RenderContext.ShutDown();
}

void CFractalRender::Render()
{
	static float t = 0.0f;
	t += ( float )XM_PI * 0.001f;

	m_World1 = XMMatrixRotationY(sin(t)*0.5)*XMMatrixRotationX(1)*XMMatrixScaling(3,3,3);
		
	m_RenderContext.Render([this](CRenderContext* pContext)
		{
			pContext->SetVertexBuffer(m_spVertexBuffer->GetBuffer(), sizeof( VERTEX ));
		
			pContext->SetIndexBuffer(m_spIndexBuffer->GetBuffer());
			
			pContext->SetVertexShaderConstantBuffer(m_spConstantBuffer->GetBuffer());

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

