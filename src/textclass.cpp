////////////////////////////////////
//Filename:textclass.cpp
////////////////////////////////////

#include<textclass.h>

TextClass::TextClass()
{
	m_Font = 0;
	m_fontShader = 0;
	m_sentence_fps = 0;
	m_sentence_cpu = 0;
	m_sentence_renderCount = 0;
}

TextClass::TextClass(const TextClass &)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext,
	HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_baseViewMatrix = baseViewMatrix;

	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	result = m_Font->Initialize(device, "data/fontdata.txt", L"data/font.dds");
	if (!result)
	{
		return false;
	}

	m_fontShader = new FontShaderClass;
	if (!m_fontShader)
	{
		return false;
	}

	result = m_fontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	result = InitializeSentence(&m_sentence_fps, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_sentence_cpu, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_sentence_renderCount, 16, device);
	if (!result)
	{
		return false;
	}
	

	return true;
}

bool TextClass::InitializeSentence(SentenceType **sentence, int maxLength, ID3D11Device *device)
{
	HRESULT result;

	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;

	VertexType *vertices;
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned long *indices;
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	//initialize the all the bits in vertex buffer's data with 0
	memset(vertices, 0, sizeof(VertexType) * (*sentence)->vertexCount);

	int i;
	for (i = 0; i != (*sentence)->indexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::Render(ID3D11DeviceContext *deviceContext, D3DXMATRIX world, D3DXMATRIX ortho)
{
	bool result;

	result = RenderSentence(deviceContext, m_sentence_fps, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence_cpu, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence_renderCount, world, ortho);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext *devCtx)
{
	char tempString[32];
	char countString[32];
	bool result;


	// Convert the count integer to string format.
	_itoa_s(renderCount, tempString, 10);

	// Setup the render count string.
	strcpy_s(countString, "RC: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence_renderCount, countString, 20, 60, 1.0f, 1.0f, 1.0f, devCtx);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext *devCtx)
{
	bool result;

	if (fps > 9999)
	{
		fps = 9999;
	}

	char tempString[16];
	_itoa_s(fps, tempString, 10);

	char fpsString[16];
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	float red, green, blue;
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else if(fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	else
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	result = UpdateSentence(m_sentence_fps, fpsString, 20, 20, red, green, blue, devCtx);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext *devCtx)
{
	bool result;

	char tempString[16];
	char cpuString[16];
	_itoa_s(cpu, tempString, 10);

	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	result = UpdateSentence(m_sentence_cpu, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, devCtx);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext *devCtx, char &typed)
{
	bool result;

	char tempString[16];
	char mouseString[16];
	_itoa_s(mouseX, tempString, 10);
	strcpy_s(mouseString, "Mouse X : ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(m_sentence_fps, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, devCtx);
	if (!result)
	{
		return false;
	}

	_itoa_s(mouseY, tempString, 10);
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(m_sentence_cpu, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, devCtx);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::RenderSentence(ID3D11DeviceContext *deviceContext, SentenceType *sentence, D3DXMATRIX world, D3DXMATRIX ortho)
{
	bool result;

	unsigned int stride, offset;
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXVECTOR4 pixelColor;
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = m_fontShader->Render(deviceContext, sentence->indexCount, world, m_baseViewMatrix, ortho, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}
}

bool TextClass::UpdateSentence(SentenceType *sentence, char *text, int positionX, int positionY, float r, float g, float b,
	ID3D11DeviceContext *deviceContext)
{
	HRESULT result;

	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	int numLetters;
	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	VertexType *vertices;
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * sentence->vertexCount);

	float drawX, drawY;
	drawX = (float)((m_screenWidth / 2)* (-1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);
	m_Font->BuildVertexArray((void *)vertices, text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mapRes;
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if (FAILED(result))
	{
		return false;
	}

	VertexType *verticesPtr;
	verticesPtr = (VertexType *)mapRes.pData;
	memcpy(verticesPtr, (void *)vertices, sizeof(VertexType) *sentence->vertexCount);
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType **sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete *sentence;
		*sentence = 0;
	}
}

void TextClass::Shutdown()
{
	ReleaseSentence(&m_sentence_fps);
	ReleaseSentence(&m_sentence_cpu);
	ReleaseSentence(&m_sentence_renderCount);

	if (m_fontShader)
	{
		m_fontShader->Shutdown();
		delete m_fontShader;
		m_fontShader = 0;
	}

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}
}

