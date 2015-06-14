////////////////////////////////////
//Filename:graphicsclass.cpp
////////////////////////////////////

#include<graphicsclass.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
	m_Model = 0;
	m_modelList = 0;
	m_lightShader = 0;
	m_Light = 0;
	m_Frustum = 0;


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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
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

	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	result = m_Model->Initialize(m_D3D->GetDevice(), L"data/seafloor.dds", "data/sphere.txt");
	if (!result)
	{
		return false;
	}

	m_lightShader = new LightShaderClass;
	if (!m_lightShader)
	{
		return false;
	}

	result = m_lightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetDirection(1.0f, 0.0f, 1.0f);

	m_modelList = new ModelListClass;
	if (!m_modelList)
	{
		return false;
	}

	result = m_modelList->Initialize(25);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ModelList", L"Error", MB_OK);
		return false;
	}

	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	//// Create the texture shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//{
	//	return false;
	//}

	//// Initialize the texture shader object.
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Create the bitmap object.
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/seafloor.dds", 10, 10);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the cursor bitmap object.", L"Error", MB_OK);
	//	return false;
	//}

	return true;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime,float rotationY)
{
	bool result;

	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0f,rotationY,0.0f);

	return true;
}

bool GraphicsClass::Render()
{
	bool result;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//generate the view matrix based on the camera's position
	m_Camera->Render();

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	//get the world ,view,and projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	int modelCount, renderCount, index;
	float posX, posY, posZ,radius;
	D3DXVECTOR4 color;
	bool renderModel;

	modelCount = m_modelList->GetModelCount();

	//model count that has been rendered
	renderCount = 0;

	for (index = 0; index != modelCount; ++index)
	{
		m_modelList->GetData(index, posX, posY, posZ, color);
		radius = 1.0f;
		renderModel = m_Frustum->CheckSphere(posX, posY, posZ, radius);
		if (renderModel)
		{
			//move the model to the location it should be rendered to
			D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

			m_Model->Render(m_D3D->GetDeviceContext());

			m_lightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTexture(), m_Light->GetDirection(), color,color,m_Camera->GetPosition(),color,1.0f);

			m_D3D->GetWorldMatrix(worldMatrix);
			renderCount++;
		}
	}

	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	/*2D rendering.¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý¡ý*/
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	/*result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}*/

	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	/*result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
	{
		return false;
	}*/

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}

void GraphicsClass::Shutdown()
{
	/*if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}*/

	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	if (m_modelList)
	{
		m_modelList->Shutdown();
		delete m_modelList;
		m_modelList = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}


	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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
