////////////////////////////////////
//Filename:fontclass.cpp
////////////////////////////////////

#include "fontclass.h"

FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device *device, char *fontFilename, WCHAR *textureFilename)
{
	bool result;

	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

bool FontClass::LoadFontData(char *filename)
{
	std::ifstream fin;
	int i;
	char temp;

	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (i = 0; i != 95; ++i)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device *device, WCHAR *filename)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

ID3D11ShaderResourceView *FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void *vertices, char *sentence, float drawX, float drawY)
{
	VertexType *vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType *)vertices;
	numLetters = (int)strlen(sentence);
	index = 0;

	//draw each letter onto a quad
	for (i = 0; i != numLetters; ++i)
	{
		letter = ((int)sentence[i] - 32);
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//first triangle
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY - 16, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY - 16, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			//second triangle
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY - 16, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			//update draw position for next letter
			drawX += m_Font[letter].size + 1.0f;
		}
	}
}

