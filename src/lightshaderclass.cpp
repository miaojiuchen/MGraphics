////////////////////////////////////
//Filename:lightshaderclass.cpp
////////////////////////////////////

#include<lightshaderclass.h>

LightShaderClass::LightShaderClass()
{
	m_layout = 0;
	m_matrixBuffer = 0;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_sampleState = 0;
	m_cameraBuffer = 0;
	m_lightBuffer = 0;
}
LightShaderClass::LightShaderClass(const LightShaderClass &){}
LightShaderClass::~LightShaderClass(){}

bool LightShaderClass::Initialize(ID3D11Device *device,HWND hwnd)
{
	bool result;

	//initialize the vertex and pixel shaders
	result = IntializeShader(device, hwnd, L"../CGraphics/lightVS.hlsl", L"../CGraphics/lightPS.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

bool LightShaderClass::IntializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vFname, WCHAR *pFname)
{
	HRESULT result;

	ID3D10Blob *errMsg;
	ID3D10Blob *vtSdrBuf;
	ID3D10Blob *pxSdrBuf;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufDesc;

	//initialize the pointers this function will use to null
	errMsg = 0;
	vtSdrBuf = 0;
	pxSdrBuf = 0;

	//compile the vertex shader code
	result = D3DX11CompileFromFile(vFname, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vtSdrBuf, &errMsg, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errMsg)
		{
			OutputShaderErrorMessage(errMsg, hwnd, vFname);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vFname, L"Missing VS Shader File", MB_OK);
		}

		return false;
	}

	//compile the pixel shader code
	result = D3DX11CompileFromFile(pFname, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pxSdrBuf, &errMsg, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errMsg)
		{
			OutputShaderErrorMessage(errMsg, hwnd, pFname);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, pFname, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//create the vertex shader from the buffer
	result = device->CreateVertexShader(vtSdrBuf->GetBufferPointer(), vtSdrBuf->GetBufferSize(),
		NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the pixel shader from the buffer
	result = device->CreatePixelShader(pxSdrBuf->GetBufferPointer(), pxSdrBuf->GetBufferSize(),
		NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the vertex input layout description
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, 
		vtSdrBuf->GetBufferPointer(), vtSdrBuf->GetBufferSize(),&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//release the vertex shader buffer and pixel shader buffer 
	vtSdrBuf->Release();
	vtSdrBuf = 0;
	pxSdrBuf->Release();
	pxSdrBuf = 0;

	//create a texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufDesc.MiscFlags = 0;
	matrixBufDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the camera dynamic constant buffer that is in the vertex shader
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	//create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the light dynamic constant buffer that is in the pixel shader
	lightBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufDesc.ByteWidth = sizeof(LightBufferType);
	lightBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufDesc.MiscFlags = 0;
	lightBufDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&lightBufDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool LightShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount,
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	ID3D11ShaderResourceView *texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition,D3DXVECTOR4 specularColor,float specularPower)
{
	bool result;

	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection,ambientColor, diffuseColor,cameraPosition,specularColor,specularPower);
	if (!result)
	{
		return false;
	}

	//render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext *dCtx,
	D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
	ID3D11ShaderResourceView *texture, D3DXVECTOR3 litDir, D3DXVECTOR4 ambientColor,D3DXVECTOR4 diffColor,D3DXVECTOR3 cameraPosition,D3DXVECTOR4 specularColor,float specularPower)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufNumber;
	MatrixBufferType *mbPtr;
	LightBufferType *lbPtr;
	CameraBufferType *cbPtr;

	//transpose the matrices to prepare them for the shader
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	//////////////////////////////
	//set matrix constant buffer//
	//////////////////////////////

	//lock the constant buffer so it can be written to
	result = dCtx->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	mbPtr = (MatrixBufferType *)mappedResource.pData;

	//copy the matrices into the constant buffer
	mbPtr->world = world;
	mbPtr->view = view;
	mbPtr->projection = projection;

	//unlock the constant buffer
	dCtx->Unmap(m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufNumber = 0;

	//now set the constant buffer in the vertex shader with the updated values
	dCtx->VSSetConstantBuffers(bufNumber, 1, &m_matrixBuffer);

	//lock the camera constant buffer so it can be written to
	result = dCtx->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the constant buffer
	cbPtr = (CameraBufferType *)mappedResource.pData;

	//copy the camera position into the constant buffer
	cbPtr->cameraPosition = cameraPosition;
	cbPtr->padding = 0.0f;

	dCtx->Unmap(m_cameraBuffer, 0);

	bufNumber = 1;

	dCtx->VSSetConstantBuffers(bufNumber, 1, &m_cameraBuffer);

	//now set shader texture resource in the pixel shader
	dCtx->PSSetShaderResources(0, 1, &texture);

	/////////////////////////////
	//set light constant buffer//
	/////////////////////////////

	//lock the light constant buffer so it can be written to
	result = dCtx->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	lbPtr = (LightBufferType *)mappedResource.pData;
	lbPtr->ambientColor = ambientColor;
	lbPtr->lightDirection = litDir;
	lbPtr->diffuseColor = diffColor;
	lbPtr->specularColor = specularColor;
	lbPtr->specularPower = specularPower;

	//unlock the constant buffer
	dCtx->Unmap(m_lightBuffer, 0);

	//set the position of the light constant buffer in the pixel shader
	bufNumber = 0;

	//finally set the light constant buffer in the pixel shader with the updated values
	dCtx->PSSetConstantBuffers(bufNumber, 1, &m_lightBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext *dCtx, int indexCount)
{
	//set the vertex input layout
	dCtx->IASetInputLayout(m_layout);

	//set the vertex and pixel shaders that will be used to render this triangle
	dCtx->VSSetShader(m_vertexShader, NULL, 0);
	dCtx->PSSetShader(m_pixelShader, NULL, 0);

	//set the sampler state in the pixel shader
	dCtx->PSSetSamplers(0, 1, &m_sampleState);

	//render the triangle
	dCtx->DrawIndexed(indexCount, 0, 0);

	return;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}

void LightShaderClass::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the camera constant buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob * errMsg, HWND hwnd, WCHAR *fName)
{
	char *compileErrors;
	unsigned long bufferSize, i;
	std::ofstream ofs;

	//get a pointer to the error message text buffer
	compileErrors = (char *)(errMsg->GetBufferPointer());

	//get the length of the message
	bufferSize = errMsg->GetBufferSize();

	//open a file to write the error message to
	ofs.open("f:\\sharder-error.txt");

	//write the error message
	for (i = 0; i != bufferSize; ++i)
	{
		ofs << compileErrors[i];
	}
	ofs << std::endl;

	ofs.close();

	//release error message
	errMsg->Release();
	errMsg = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader file.Please check f:\shader-error.txt for more information!", fName, MB_OK);

	return;
}