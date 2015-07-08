////////////////////////////////////
//Filename:modelclass.cpp
////////////////////////////////////

#include<modelclass.h>

ModelClass::ModelClass()
{
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
	m_Textures = 0;
	m_Model = 0;
}

ModelClass::ModelClass(const ModelClass &)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device *device, char *modelFilename, WCHAR *textureFilename1, WCHAR * textureFilename2, WCHAR *textureFilename3)
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
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
	HRESULT result;

	VertexType *vertices;
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned long *indices;
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	int i;
	for (i = 0; i != m_vertexCount; ++i)
	{
		vertices[i].position = D3DXVECTOR3(m_Model[i].pos_x, m_Model[i].pos_y, m_Model[i].pos_z);
		vertices[i].texture = D3DXVECTOR2(m_Model[i].tex_u, m_Model[i].tex_v);
		vertices[i].normal = D3DXVECTOR3(m_Model[i].nor_x, m_Model[i].nor_y, m_Model[i].nor_z);
		vertices[i].tangent = D3DXVECTOR3(m_Model[i].tan_x, m_Model[i].tan_y, m_Model[i].tan_z);
		vertices[i].binormal = D3DXVECTOR3(m_Model[i].bin_x, m_Model[i].bin_y, m_Model[i].bin_z);

		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_vertexCount * sizeof(VertexType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indexCount * sizeof(unsigned long);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
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


void ModelClass::Shutdown()
{
	//release the model texture
	ReleaseTextures();

	//release the vertex and index buffers
	ShutdownBuffers();

	//release model data
	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
	//put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView **ModelClass::GetTextureArray()
{
	return m_Textures->GetTextureArray();
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
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
}

bool ModelClass::LoadTextures(ID3D11Device *device, WCHAR *filename1, WCHAR *filename2, WCHAR *filename3)
{
	bool result;

	//create the texture object
	m_Textures = new TextureArrayClass;
	if (!m_Textures)
	{
		return false;
	}

	//initialize the texture array object
	result = m_Textures->Initialize(device, filename1, filename2, filename3);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTextures()
{
	if (m_Textures)
	{
		m_Textures->Shutdown();
		delete m_Textures;
		m_Textures = 0;
	}
}

void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	faceCount = m_vertexCount / 3;
	index = 0;

	for (i = 0; i != faceCount; ++i)
	{
		loadStruct(vertex1, m_Model, index++);
		loadStruct(vertex2, m_Model, index++);
		loadStruct(vertex3, m_Model, index++);

		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);
		CalculateNormal(tangent, binormal, normal);

		loadModel(m_Model, index, tangent, binormal, normal);
	}
}

inline void ModelClass::loadStruct(TempVertexType &vertex1, ModelType *m_Model, int index)
{
	vertex1.pos_x = m_Model[index].pos_x;
	vertex1.pos_y = m_Model[index].pos_y;
	vertex1.pos_z = m_Model[index].pos_z;
	vertex1.tex_u = m_Model[index].tex_u;
	vertex1.tex_v = m_Model[index].tex_v;
	vertex1.nor_x = m_Model[index].nor_x;
}

inline void ModelClass::loadModel(ModelType *m_model, int index, VectorType &tangent, VectorType &binormal, VectorType &normal)
{
	int i;
	for (i = 1; i != 4; ++i)
	{
		m_model[index - i].nor_x = normal.x;
		m_model[index - i].nor_y = normal.y;
		m_model[index - i].nor_z = normal.z;
		m_model[index - i].tan_x = tangent.x;
		m_model[index - i].tan_y = tangent.y;
		m_model[index - i].tan_z = tangent.z;
		m_model[index - i].bin_x = binormal.x;
		m_model[index - i].bin_y = binormal.y;
		m_model[index - i].bin_z = binormal.z;
	}
}

void ModelClass::CalculateTangentBinormal(TempVertexType vt1, TempVertexType vt2, TempVertexType vt3, VectorType &tangent, VectorType &binormal)
{
	float vec1[3], vec2[3];
	float tuVec[2], tvVec[2];
	float den;
	float length;

	vec1[0] = vt2.pos_x - vt1.pos_x;
	vec1[1] = vt2.pos_y - vt2.pos_y;
	vec1[2] = vt2.pos_z - vt1.pos_z;

	vec2[0] = vt3.pos_x - vt1.pos_x;
	vec2[1] = vt3.pos_y - vt1.pos_y;
	vec2[2] = vt3.pos_z - vt1.pos_z;

	tuVec[0] = vt2.tex_u - vt1.tex_u;
	tvVec[0] = vt2.tex_v - vt1.tex_v;

	tuVec[1] = vt3.tex_u - vt1.tex_u;
	tvVec[1] = vt3.tex_v - vt1.tex_v;

	den = 1.0f / (tuVec[0] * tvVec[1] - tuVec[1] * tvVec[0]);

	tangent.x = (tvVec[1] * vec1[0] - tvVec[0] * vec2[0]) * den;
	tangent.y = (tvVec[1] * vec1[1] - tvVec[0] * vec2[1]) *den;
	tangent.z = (tvVec[1] * vec1[2] - tvVec[0] * vec2[2]) *den;

	binormal.x = (tuVec[0] * vec2[0] - tuVec[1] * vec1[0]) * den;
	binormal.y = (tuVec[0] * vec2[1] - tuVec[1] * vec1[1])*den;
	binormal.z = (tuVec[0] * vec2[2] - tuVec[1] * vec1[2])*den;

	normalizeVertex(tangent);
	normalizeVertex(binormal);
}

inline void ModelClass::normalizeVertex(VectorType &tangent)
{
	float length;
	length = sqrt(tangent.x * tangent.x + tangent.y*tangent.y + tangent.z*tangent.z);
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;
}

void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType &normal)
{
	float length;

	normal.x = tangent.y*binormal.z - tangent.z*binormal.y;
	normal.y = tangent.z*binormal.x - tangent.x*binormal.z;
	normal.z = tangent.x*binormal.y - tangent.y*binormal.x;

	normalizeVertex(normal);
}