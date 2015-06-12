////////////////////////////////////
//Filename:systemclass.h
////////////////////////////////////

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

//linking instructions
#define WIN32_LEAN_AND_MEAN

//includes
#include<windows.h>
#include<inputclass.h>
#include<graphicsclass.h>
#include<soundclass.h>

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass &);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int &, int &);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass *m_Input;
	GraphicsClass *m_Graphics;
	SoundClass *m_Sound;

};

//function prototypes
static LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

//globals
static SystemClass *ApplicationHandle = 0;

#endif