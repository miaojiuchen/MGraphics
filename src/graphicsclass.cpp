////////////////////////////////////
//Filename:graphicsclass.cpp
////////////////////////////////////

#include<graphicsclass.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
	m_Bitmap = 0;
	m_TextureShader = 0;
	/*
	m_Model = 0;
	m_lightShader = 0;
	m_Light = 0;
	*/
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//create the Direct3D object
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	//initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	//set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f,-1.0f);
	m_Camera->Render();

	D3DXMATRIX baseViewMatrix;
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	////create the model object
	//m_Model = new ModelClass;
	//if (!m_Model)
	//{
	//	return false;
	//}

	////initialize the model object
	//result = m_Model->Initialize(m_D3D->GetDevice(),"../CGraphics/data/model.txt", L"../CGraphics/data/seafloor.dds");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the texture object from file", L"Error", MB_OK);
	//	return false;
	//}

	////create the light shader object
	//m_lightShader = new LightShaderClass;
	//if (!m_lightShader)
	//{
	//	return false;
	//}

	////initialize the light shader object
	//result = m_lightShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
	//	return false;
	//}

	////Create the light object.
	//m_Light = new LightClass;
	//if (!m_Light)
	//{
	//	return false;
	//}

	//// Initialize the light object.
	//m_Light->SetAmbientColor(0.0f, 0.30f, 0.0f, 1.0f);
	//m_Light->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	//m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	//m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularPower(16.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/seafloor.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cursor bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool GraphicsClass::Frame(int mouseX,int mouseY,char &typed)
{
	bool result;

	//static float rotation = 0.0f;

	////updata the rotation variable each frame
	//rotation += (float)D3DX_PI*0.005f;
	//if (rotation > 360.0f)
	//{
	//	rotation -= 360.0f;
	//}

	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext(),typed);
	if (!result)
	{
		return false;
	}

	result = Render(mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(int mouseX,int mouseY)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix,orthoMatrix;
	bool result;

	//clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//generate the view matrix based on the camera's position
	m_Camera->Render();

	//get the world ,view,and projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	////rotate the world matrix by the rotation value so  that the triangle will spin
	//D3DXMatrixRotationY(&worldMatrix, rotation);

	////put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	//m_Model->Render(m_D3D->GetDeviceContext());

	////render the model using the texture shader
	//result = m_lightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
	//	worldMatrix, viewMatrix, projectionMatrix, 
	//	m_Model->GetTexture(),m_Light->GetDirection(),m_Light->GetAmbientColor(),
	//	m_Light->GetDiffuseColor(),m_Camera->GetPosition(),m_Light->GetSpecularColor(),m_Light->GetSpecularPower());
	//if (!result)
	//{
	//	return false;
	//}

	/*2D rendering.¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý*/
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	/*2D rendering completed.¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü*/
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	//present the rendered scene to the screen
	m_D3D->EndScene();
	return true;
}

void GraphicsClass::Shutdown()
{
	////Release the light object.
	//if (m_Light)
	//{
	//	delete m_Light;
	//	m_Light = 0;
	//}

	//// Release the light shader object.
	//if (m_lightShader)
	//{
	//	m_lightShader->Shutdown();
	//	delete m_lightShader;
	//	m_lightShader = 0;
	//}

	////release the model object
	//if (m_Model)
	//{
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = 0;
	//}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	//release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}
