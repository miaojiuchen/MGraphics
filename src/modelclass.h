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
#include<texturearrayclass.h>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
	struct ModelType
	{
		float pos_x, pos_y, pos_z;
		float tex_u, tex_v;
		float nor_x, nor_y, nor_z;
		float tan_x, tan_y, tan_z;
		float bin_x, bin_y, bin_z;
	};
	struct TempVertexType
	{
		float pos_x, pos_y, pos_z;
		float tex_u, tex_v;
		float nor_x, nor_y, nor_z;
	};
	struct VectorType
	{
		float x, y, z;
	};

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Initialize(ID3D11Device *pdev, char *pmodelFilename, WCHAR *ptextureFilename1, WCHAR *ptextureFilename2, WCHAR *ptextureFilename3);
	void Shutdown();
	void Render(ID3D11DeviceContext *pdevCtx);

	int GetIndexCount();
	ID3D11ShaderResourceView **GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device *pdev);
	void RenderBuffers(ID3D11DeviceContext *pdevCtx);
	void ShutdownBuffers();

	bool LoadTextures(ID3D11Device *, WCHAR *, WCHAR *, WCHAR *);
	void ReleaseTextures();

	bool LoadModel(char *);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType &, VectorType &);
	void CalculateNormal(VectorType, VectorType, VectorType&);
	inline void loadStruct(TempVertexType &, ModelType *, int);
	inline void loadModel(ModelType *m_Model, int index, VectorType &, VectorType &, VectorType &);
	inline void normalizeVertex(VectorType &);

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	TextureArrayClass *m_Textures;
	ModelType *m_Model;
};

#endif