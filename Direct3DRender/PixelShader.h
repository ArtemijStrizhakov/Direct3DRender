#pragma once

#include <memory>
#include <string>

class ID3D11PixelShader;
class ID3D11Device;

class CPixelShader
{
public:
	CPixelShader();
	virtual ~CPixelShader();

	typedef std::shared_ptr<CPixelShader> Ptr;

	bool Compile(std::wstring const & shaderPath, ID3D11Device* pDevice);

	ID3D11PixelShader*	GetShader() { return m_pPixelShader; }

protected:

	ID3D11PixelShader*	m_pPixelShader;;

};

