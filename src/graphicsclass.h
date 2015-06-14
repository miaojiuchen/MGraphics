////////////////////////////////////
//Filename:graphicsclass.h
////////////////////////////////////

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//includes
#include<windows.h>

//my includes
#include<d3dclass.h>
#include<cameraclass.h>
#include<textclass.h>
#include<modelclass.h>
#include<modellistclass.h>
#include<frustumclass.h>
#include<lightclass.h>
#include<lightshaderclass.h>

//globals
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float,float rotationY);
	bool Render();

private:
	D3DClass *m_D3D;
	CameraClass *m_Camera;
	ModelClass *m_Model;
	ModelListClass *m_modelList;
	TextClass *m_Text;
	LightClass *m_Light;
	LightShaderClass *m_lightShader;
	FrustumClass *m_Frustum;

};

#endif