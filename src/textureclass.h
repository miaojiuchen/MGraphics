////////////////////////////////////
//Filename:textureclass.h
////////////////////////////////////

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

//includes
#include<d3d11.h>
#include<d3dx11tex.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass &);
	~TextureClass();

	bool Initialize(ID3D11Device *, WCHAR *);
	void Shutdown();

	ID3D11ShaderResourceView *GetTexture();
private:
	ID3D11ShaderResourceView *m_texture;
};

#endif
