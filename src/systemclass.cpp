////////////////////////////////////
//Filename:systemclass.cpp
////////////////////////////////////

#include<systemclass.h>

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Sound = 0;
}

SystemClass::SystemClass(const SystemClass &)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	bool result;

	int screenWidth, screenHeight;
	screenWidth = screenHeight = 0;
	//Initialize the windows api
	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could'n initialize the input object", L"Error", MB_OK);
		return false;
	}

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound", L"Error", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}


/*
 *	PSEUDO CODE:
 *	While not done yet
 *	Check for windows system messages
 *	Process system messages
 *	Process application loop
 *	Check if user wanted to quit durign the frame processiong
 */
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	//initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//loop until there is a quit message from the window or the user
	done = false;
	while (!done)
	{
		//handle the windows messages
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		if (m_Input->isEscapePressed())
		{
			done = true;
		}
	}
}

bool SystemClass::Frame()
{
	bool result;

	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	int mouseX, mouseY;
	m_Input->GetMouseLocation(mouseX, mouseY);

	char whatTyped = 'X';
	m_Input->GetWhatTyped(whatTyped);

	result = m_Graphics->Frame(mouseX, mouseY, whatTyped);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

void SystemClass::InitializeWindows(int &screenWidth, int &screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object
	ApplicationHandle = this;

	//get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//give the application a name
	m_applicationName = L"M_CG";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determin the resolution of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//setup the screen settings depending on whether it is running in full screen or in windowed mode
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		//if windowed then set it to 800*600 resolution
		screenWidth = 800;
		screenHeight = 600;

		//place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//create the window with the screen settings and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//hide the mouse cursor
	ShowCursor(true);
}

void SystemClass::ShutdownWindows()
{
	//show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = 0;

	//remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = 0;

	//release the pointer to this class
	ApplicationHandle = 0;
}

