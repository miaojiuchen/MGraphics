////////////////////////////////////
//Filename:fontshaderclass.cpp
////////////////////////////////////

#include "FontShaderClass.h"

FontShaderClass::FontShaderClass()
{
	m_constantBuffer = 0;
	m_Layout = 0;
	m_pixelBuffer = 0;
	m_pixelShader = 0;
	m_sampleState = 0;
	m_sampleState = 0;
}

FontShaderClass::~FontShaderClass()
{
}

bool FontShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"fontVS.hlsl", L"fontPS.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void FontShaderClass::Shutdown()
{
	ShutdownShader();
}

bool FontShaderClass::Render(ID3D11DeviceContext *dctx, int indexCount,
	D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	ID3D11ShaderResourceView *texture, D3DXVECTOR4 pixelColor)
{
	bool result;

	result = SetShaderParameters(dctx, world, view, projection, texture, pixelColor);
	if (!result)
	{
		return false;
	}

	RenderShader(dctx, indexCount);

	return true;
}

bool FontShaderClass::InitializeShader(ID3D11Device *dev, HWND hwnd, WCHAR *vsFilename, WCHAR *psFilename)
{
	HRESULT result;

	ID3D10Blob *errMsg = 0;
	ID3D10Blob *vtSdBuf = 0;
	ID3D10Blob *pxSdBuf = 0;
	result = D3DX11CompileFromFile(vsFilename, 0, 0, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vtSdBuf, &errMsg, 0);
	if (FAILED(result))
	{
		if (errMsg)
		{
			OutputShaderErrorMessage(errMsg, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing vs Shader Filename", MB_OK);
		}
		return false;
	}

	result = D3DX11CompileFromFile(psFilename, 0, 0, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 0,
		&pxSdBuf, &errMsg, 0);
	if (FAILED(result))
	{
		if (errMsg)
		{
			OutputShaderErrorMessage(errMsg, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing ps Shader Filename", MB_OK);
		}
		return false;
	}

	result = dev->CreateVertexShader(vtSdBuf->GetBufferPointer(), vtSdBuf->GetBufferSize(), 0, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = dev->CreatePixelShader(pxSdBuf->GetBufferPointer(), pxSdBuf->GetBufferSize(), 0, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int elemNum;
	elemNum = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create vertex input layout
	result = dev->CreateInputLayout(polygonLayout, elemNum, vtSdBuf->GetBufferPointer(), vtSdBuf->GetBufferSize(), &m_Layout);
	if (FAILED(result))
	{
		return false;
	}

	vtSdBuf->Release();
	vtSdBuf = 0;
	pxSdBuf->Release();
	pxSdBuf = 0;

	D3D11_BUFFER_DESC constBufDesc;
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.ByteWidth = sizeof(ConstantBufferType);
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;

	result = dev->CreateBuffer(&constBufDesc, NULL, &m_constantBuffer);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = dev->CreateSamplerState(&sampDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC pxBufDesc;
	pxBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	pxBufDesc.ByteWidth = sizeof(PixelBufferType);
	pxBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pxBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pxBufDesc.MiscFlags = 0;
	pxBufDesc.StructureByteStride = 0;

	result = dev->CreateBuffer(&pxBufDesc, NULL, &m_pixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FontShaderClass::ShutdownShader()
{
	if (m_pixelBuffer)
	{
		m_pixelBuffer->Release();
		m_pixelBuffer = 0;
	}

	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void FontShaderClass::OutputShaderErrorMessage(ID3D10Blob *errMsg, HWND hwnd, WCHAR * shaderFilename)
{
	char *compileErrors;
	unsigned long bufferSize, i;

	compileErrors = (char *)(errMsg->GetBufferPointer());
	bufferSize = errMsg->GetBufferSize();

	std::ofstream fout;
	fout.open("shader-error.txt");
	for (i = 0; i != bufferSize; ++i)
	{
		fout << compileErrors[i];
	}
	fout.close();
	errMsg->Release();
	errMsg = 0;
	MessageBox(hwnd, L"Error compilinig shader. Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool FontShaderClass::SetShaderParameters(ID3D11DeviceContext *devCtx,
	D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,
	ID3D11ShaderResourceView *texture, D3DXVECTOR4 pxColor)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = devCtx->Map(m_constantBuffer, NULL,D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	ConstantBufferType *dataPtr;
	dataPtr = (ConstantBufferType *)mappedResource.pData;
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	devCtx->Unmap(m_constantBuffer,NULL);

	unsigned int bufferNum;
	bufferNum = 0;
	devCtx->VSSetConstantBuffers(bufferNum, 1, &m_constantBuffer);
	devCtx->PSSetShaderResources(0, 1, &texture);


	result = devCtx->Map(m_pixelBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	PixelBufferType *dataPtr2;
	dataPtr2 = (PixelBufferType *)mappedResource.pData;
	dataPtr2->pixelColor = pxColor;
	devCtx->Unmap(m_pixelBuffer, 0);

	bufferNum = 0;
	devCtx->PSSetConstantBuffers(bufferNum, 1, &m_pixelBuffer);

	return true;
}

void FontShaderClass::RenderShader(ID3D11DeviceContext *devCtx, int indexCount)
{
	devCtx->IASetInputLayout(m_Layout);
	devCtx->VSSetShader(m_vertexShader, 0, 0);
	devCtx->PSSetShader(m_pixelShader, 0, 0);
	devCtx->PSSetSamplers(0, 1, &m_sampleState);

	//draw
	devCtx->DrawIndexed(indexCount, 0, 0);
}


