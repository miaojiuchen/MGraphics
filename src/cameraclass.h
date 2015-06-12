////////////////////////////////////
// Filename:cameraclass.h
// CameraClass will take care of our view matrix
// It will handle the location of the camera in the world and pass it to shaders 
// when they need to draw and figure out where we are looking at the scene from. 
////////////////////////////////////

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

//includes
#include<d3dx10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass &);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX &);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif