#pragma once

#include <memory>
#include <string>

class ID3D11GeometryShader;
class ID3D11Device;

class CGeometryShader
{
public:
	CGeometryShader();
	~CGeometryShader();

	typedef std::shared_ptr<CGeometryShader> Ptr;

	bool Compile(std::wstring const & shaderPath, ID3D11Device* pDevice);

	ID3D11GeometryShader*	GetShader() { return m_pGeometryShader; }

protected:

	ID3D11GeometryShader*	m_pGeometryShader;

};
