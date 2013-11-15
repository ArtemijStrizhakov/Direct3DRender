#pragma once

#include <memory>
#include <string>

class ID3D11VertexShader;
class ID3D11Device;
class ID3D11InputLayout;

class CVertexShader
{
public:
	CVertexShader();
	virtual ~CVertexShader();

	typedef std::shared_ptr<CVertexShader> Ptr;

	bool Compile(std::wstring const & shaderPath, ID3D11Device* pDevice);

	ID3D11VertexShader* GetShader() { return m_pVertexShader; }
	ID3D11InputLayout* GetLayout() { return m_pVertexLayout; }
	
protected:

	ID3D11VertexShader*	m_pVertexShader;
	ID3D11InputLayout*  m_pVertexLayout;

};



