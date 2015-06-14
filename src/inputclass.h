////////////////////////////////////
//Filename:inputclass.h
////////////////////////////////////

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

//linking instructions
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//includes
#include<dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass &);
	~InputClass();

	bool Initialize(HINSTANCE,HWND,int,int);
	void Shutdown();
	bool Frame();

	bool isEscapePressed();
	bool isLeftArrowPressed();
	bool isRightArrowPressed();
	void GetMouseLocation(int &,int &);
	void GetWhatTyped(char &);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	//bool m_keys[256];
	IDirectInput8 *m_directInput;
	IDirectInputDevice8 *m_keyboard;
	IDirectInputDevice8 *m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

};

#endif

