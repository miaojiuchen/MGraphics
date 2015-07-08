////////////////////////////////////
// Filename:specularmapshaderclass.h
////////////////////////////////////

#ifndef _SPECULARMAPSHADERCLASS_H_
#define _SPECULARMAPSHADERCLASS_H_

#include<D3D11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include<fstream>

class SpecularmapshaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR4 specularColor;
		float specularPower;
		D3DXVECTOR3 lightDirection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};
public:
	SpecularmapshaderClass();
	SpecularmapshaderClass(const SpecularmapshaderClass &);
	~SpecularmapshaderClass();

	bool Initialize(ID3D11Device *pdev, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext *pdevCtx, int indexCount, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView **pTextures, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float speculrPower);

private:
	bool InitializeShader(ID3D11Device *pdev, HWND hwdn, WCHAR *filename1, WCHAR *filename2);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float speculrPower);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer *m_lightBuffer;
	ID3D11Buffer *m_cameraBuffer;

};

#endif



