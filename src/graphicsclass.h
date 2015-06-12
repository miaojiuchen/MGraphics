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
#include<bitmapclass.h>
#include<textureshaderclass.h>

//globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0F;
const float SCREEN_NEAR = 0.1F;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, char &);
	bool Render(int mouseX, int mouseY);

private:
	D3DClass *m_D3D;
	CameraClass *m_Camera;
	TextClass *m_Text;
	BitmapClass *m_Bitmap;
	TextureShaderClass *m_TextureShader;
	/*
	ModelClass *m_Model;
	LightShaderClass *m_lightShader;
	LightClass *m_Light;
	*/
};

#endif