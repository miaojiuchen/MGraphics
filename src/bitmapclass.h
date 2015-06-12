////////////////////////////////////
//Filename:bitmapclass.h
////////////////////////////////////

#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_

//includes
#include<D3D11.h>
#include<D3DX10math.h>

#include<textureclass.h>

class BitmapClass
{
private:
	struct Cursor
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount;
		D3DXVECTOR3 position;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass &);
	~BitmapClass();

	bool Initialize(ID3D11Device *, int, int, WCHAR *, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext *, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView *GetTexture();

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext *,int,int);
	void RenderBuffers(ID3D11DeviceContext *);

	bool LoadTexture(ID3D11Device *, WCHAR *);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	TextureClass *m_Texture;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;

private:

};

#endif

