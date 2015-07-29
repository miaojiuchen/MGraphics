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
#include<lightclass.h>
#include<lightshaderclass.h>
#include<rendertextureclass.h>
#include<refractionshaderclass.h>
#include<watershaderclass.h>

//globals
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int MODEL_NUM = 4;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int,float,float);
	bool Render();

private:
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene();
	
private:
	float m_waterHeight, m_waterTranslation;

	D3DClass *m_D3D;
	CameraClass *m_Camera;
	ModelClass *m_GroundModel,*m_WallModel,*m_BathModel,*m_WaterModel;
	LightClass *m_Light;
	LightShaderClass *m_LightShader;
	RenderTextureClass *m_RefractionTexture,*m_ReflectionTexture;
	RefractionShaderClass *m_RefractionShader;
	WaterShaderClass *m_WaterShader;

};

#endif