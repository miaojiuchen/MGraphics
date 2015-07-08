////////////////////////////////////
//Filename:texturearrayclass.cpp
////////////////////////////////////

#include "texturearrayclass.h"

TextureArrayClass::TextureArrayClass()
{
	m_Textures[0] = 0;
	m_Textures[1] = 0;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass &)
{
}

TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device *pdev, WCHAR *filename1, WCHAR *filename2,WCHAR *filename3)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(pdev, filename1, 0, 0, &m_Textures[0], 0);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(pdev, filename2, 0, 0, &m_Textures[1], 0);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(pdev, filename3, 0, 0, &m_Textures[2], 0);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureArrayClass::Shutdown()
{
	if (m_Textures[0])
	{
		m_Textures[0]->Release();
		m_Textures[0] = 0;
	}

	if (m_Textures[1])
	{
		m_Textures[1]->Release();
		m_Textures[1] = 0;
	}

	if (m_Textures[2])
	{
		m_Textures[2]->Release();
		m_Textures[2] = 0;
	}
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_Textures;
}
