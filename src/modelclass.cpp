////////////////////////////////////
//Filename:modelclass.cpp
////////////////////////////////////

#include<modelclass.h>

ModelClass::ModelClass()
{
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
	m_Texture = 0;
	m_Model = 0;
	//m_colorShader = 0;
}
ModelClass::ModelClass(const ModelClass &rhs){}
ModelClass::~ModelClass(){}

bool ModelClass::Initialize(ID3D11Device *device, WCHAR *textureFilename,char * modelFilename)
{
	bool result;

	//load model data
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	//intialize the vertex and index buffer that hold the geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//load the texture for this model
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	//release the model texture
	ReleaseTexture();

	//release the vertex and index buffers
	ShutdownBuffers();

	//release model data
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
	//put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView *ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//load the vertex array with data
	for (i = 0; i != m_vertexCount; ++i)
	{
		vertices[i].position = D3DXVECTOR3(m_Model[i].pos_x, m_Model[i].pos_y, m_Model[i].pos_z);
		vertices[i].texture = D3DXVECTOR2(m_Model[i].tex_u, m_Model[i].tex_v);
		vertices[i].normal = D3DXVECTOR3(m_Model[i].nor_x, m_Model[i].nor_y, m_Model[i].nor_z);

		indices[i] = i;
	}

	//set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_vertexCount * sizeof(VertexType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indexCount * sizeof(unsigned long);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//release the arrays now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	//release the index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//release the vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set the type of primitive that should be rendered from this  vertex buffer,in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
bool ModelClass::LoadModel(char *filename)
{
	std::ifstream ifs;
	char input;
	int i;

	ifs.open(filename);

	if (ifs.fail())
	{
		return false;
	}

	//read in the vertex count
	ifs.get(input);
	while (input != ':')
	{
		ifs.get(input);
	}

	ifs >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_Model = new ModelType[m_vertexCount];

	if (!m_Model)
	{
		return false;
	}

	ifs.get(input);

	while (input != ':')
	{
		ifs.get(input);
	}

	ifs.get(input);
	ifs.get(input);

	//read in the vertex data
	for (i = 0; i != m_vertexCount; ++i)
	{
		ifs >> m_Model[i].pos_x >> m_Model[i].pos_y >> m_Model[i].pos_z;
		ifs >> m_Model[i].tex_u >> m_Model[i].tex_v;
		ifs >> m_Model[i].nor_x >> m_Model[i].nor_y >> m_Model[i].nor_z;
	}

	ifs.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = 0;
	}

	return;
}

bool ModelClass::LoadTexture(ID3D11Device *device, WCHAR *filename)
{
	bool result;

	//create the texture object
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//initialize the texture object
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	//release the texture object
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
