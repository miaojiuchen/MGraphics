////////////////////////////////////
//Filename:multitextureshader.h
////////////////////////////////////

#ifndef _MULTITEXTURESHADER_H_
#define _MULTITEXTURESHADER_H_

//includes
#include<D3D11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include<fstream>

class MultitextureshaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};
public:
	MultitextureshaderClass();
	MultitextureshaderClass(const MultitextureshaderClass &);
	~MultitextureshaderClass();

	bool Initialize(ID3D11Device *dev,HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext *ipdevCtx, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView **pptexture);

private:
	bool InitializeShader(ID3D11Device *ipdev, HWND hwnd, WCHAR *vsFilename, WCHAR *psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *pErrMsg, HWND hwnd, WCHAR *);

	bool SetShaderParameters(ID3D11DeviceContext*ipdevCtx, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView**pptexture);
	void RenderShader(ID3D11DeviceContext*ipdevCtx, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

};

#endif
