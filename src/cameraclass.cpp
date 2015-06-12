////////////////////////////////////
//Filename:cameraclass.cpp
////////////////////////////////////

#include<cameraclass.h>

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}
CameraClass::CameraClass(const CameraClass &){}
CameraClass::~CameraClass(){}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	//setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//setup the position of the camera in the world;
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	//setup where the camera is looking by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	float yaw, pitch, roll;
	//set the yaw(Y axis),pitch(X axis),roll(Z axis) rotations in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	D3DXMATRIX rotationMatrix;
	//create the rotation matrix from the yaw,pitch and roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//transfrom the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//translate the rotated camera position to the location of the viewer
	lookAt += position;

	//finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}