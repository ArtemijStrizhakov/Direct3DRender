#include "stdafx.h"
#include "Render.h"
#include <xnamath.h>


CRender::CRender()
{

}


CRender::~CRender()
{

}

bool CRender::Initialize( HWND hWnd, int nWidth, int nHeigth )
{
	m_RC.Initialize(hWnd, nWidth, nHeigth);
	
	m_PS = m_RC.CreatePixelShader(L"shaders.fx");
	
	m_VS = m_RC.CreateVertexShader(L"shaders.fx");

	//////////////////////////////////////////////////////////////////////////

	m_VB = m_RC.CreateBuffer<VERTEX>(VertexBuffer);
		
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, -1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, -1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( 1.0f, -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->m_Items.push_back( VERTEX( XMFLOAT4( -1.0f, -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ));
	m_VB->Compile();

	//////////////////////////////////////////////////////////////////////////

	m_IB = m_RC.CreateBuffer<WORD>(IndexBuffer);

	WORD indexes[] = {3,1,0, 2,1,3,	0,5,4, 1,5,0, 3,4,7, 0,4,3, 1,6,5, 2,6,1, 2,7,6, 3,7,2, 6,4,5, 7,4,6,};

	std::vector<WORD> v(indexes, indexes + sizeof(indexes) / sizeof(WORD));

	m_IB->m_Items = v;

	m_IB->Compile();

	//////////////////////////////////////////////////////////////////////////

	m_CB = m_RC.CreateBuffer<CONSTANTBUFER>(ConstantBuffer);

	m_Constants.world = XMMatrixIdentity();
	m_Constants.view = XMMatrixLookAtLH( XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f ), XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ), XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
	m_Constants.projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, nWidth / (FLOAT)nHeigth, 0.01f, 100.0f );
		
	m_CB->m_Items.push_back(m_Constants);
	
	m_CB->Compile();

	return true;
}

void CRender::ShutDown()
{
	m_VB.reset();
	m_PS.reset();
	m_VS.reset();

	m_RC.ShutDown();
}

void CRender::Render()
{
	static float t = 0.0f;
	t += ( float )XM_PI * 0.000125f;

	m_Constants.world = XMMatrixRotationY( t );

	m_CB->m_Items[0] = m_Constants.PrepareForBuffer();

	m_CB->Update();


	m_RC.Render([this](ID3D11DeviceContext* pContext)
		{
			UINT stride = sizeof( VERTEX );
			UINT offset = 0;
						
			std::vector<ID3D11Buffer*> buffers;
			buffers.push_back(m_VB->GetBuffer());
			ID3D11Buffer *const * ppBuffers = (ID3D11Buffer *const *)buffers.data(); 
			pContext->IASetVertexBuffers( 0, buffers.size(), ppBuffers, &stride, &offset );
		
			pContext->IASetIndexBuffer( m_IB->GetBuffer(), DXGI_FORMAT_R16_UINT, 0 );

			buffers.clear();
			buffers.push_back(m_CB->GetBuffer());
			ppBuffers = (ID3D11Buffer *const *)buffers.data(); 
			pContext->VSSetConstantBuffers( 0, 1, ppBuffers );

			pContext->PSSetShader( m_PS->GetShader(), NULL, 0 );
			pContext->VSSetShader( m_VS->GetShader(), NULL, 0 );
			pContext->IASetInputLayout( m_VS->GetLayout() );

			pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			pContext->DrawIndexed( m_IB->m_Items.size(), 0, 0 );
		});
}

