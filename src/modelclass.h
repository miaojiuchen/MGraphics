////////////////////////////////////
//Filename:modelclass.h
////////////////////////////////////

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//inlcudes
#include<d3d11.h>
#include<d3dx10math.h>
#include<fstream>

//my includes
#include<textureclass.h>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	struct ModelType
	{
		float pos_x, pos_y, pos_z;
		float tex_u, tex_v;
		float nor_x, nor_y, nor_z;
	};
public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Initialize(ID3D11Device *, WCHAR *, char *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

	ID3D11ShaderResourceView *GetTexture();
private:
	bool InitializeBuffers(ID3D11Device *);
	void RenderBuffers(ID3D11DeviceContext *);
	void ShutdownBuffers();

	bool LoadTexture(ID3D11Device *, WCHAR *);
	void ReleaseTexture();

	bool LoadModel(char *);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	TextureClass *m_Texture;
	ModelType *m_Model;
};

#endif