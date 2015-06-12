////////////////////////////////////
//Filename:lightclass.cpp
////////////////////////////////////

#include<lightclass.h>

LightClass::LightClass(){}
LightClass::LightClass(const LightClass &){}
LightClass::~LightClass(){}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetSpecularPower(float p)
{
	m_specularPower = p;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}