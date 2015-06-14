////////////////////////////////////
//Filename:modellistclass.cpp
////////////////////////////////////

#include "modellistclass.h"

ModelListClass::ModelListClass()
{
}

ModelListClass::ModelListClass(const ModelListClass &)
{
}

ModelListClass::~ModelListClass()
{
}

bool ModelListClass::Initialize(int num)
{

	m_modelCount = num;

	//create a list array of the model info
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if (!m_ModelInfoList)
	{
		return false;
	}

	srand((unsigned int)time(0));

	int i;
	float r, g, b;
	for (i = 0; i != m_modelCount; ++i)
	{
		r = (float)rand() / RAND_MAX;
		g = (float)rand() / RAND_MAX;
		b = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = D3DXVECTOR4(r, g, b, 1.0f);

		m_ModelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) *10.0f;
		m_ModelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) *10.0f;
		m_ModelInfoList[i].positionZ = (((float)rand() - (float)rand()) / RAND_MAX) *10.0f + 5.0f;
	}

	return true;
}

void ModelListClass::Shutdown()
{
	if (m_ModelInfoList)
	{
		delete[] m_ModelInfoList;
		m_ModelInfoList = 0;
	}
}

int ModelListClass::GetModelCount()
{
	return m_modelCount;
}

void ModelListClass::GetData(int i, float &posX, float &posY, float &posZ, D3DXVECTOR4 &color)
{
	posX = m_ModelInfoList[i].positionX;
	posY = m_ModelInfoList[i].positionY;
	posZ = m_ModelInfoList[i].positionZ;
	color = m_ModelInfoList[i].color;
}
