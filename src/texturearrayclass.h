////////////////////////////////////
//Filename:texturearrayclass.h
////////////////////////////////////

#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

//includes
#include<D3D11.h>
#include<D3DX11tex.h>

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass &);
	~TextureArrayClass();

	bool Initialize(ID3D11Device *pdev, WCHAR *pFilename1, WCHAR *pFilename2,WCHAR *);
	void Shutdown();

	ID3D11ShaderResourceView **GetTextureArray();

private:
	ID3D11ShaderResourceView *m_Textures[3];

};

#endif

