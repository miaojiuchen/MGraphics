////////////////////////////////////
//Filename:graphicsclass.cpp
////////////////////////////////////

#include<graphicsclass.h>

GraphicsClass::GraphicsClass()
{
	//base stuffs
	m_D3D = 0;
	m_Camera = 0;
	m_Light = 0;

	//models
	m_GroundModel = 0;
	m_BathModel = 0;
	m_WallModel = 0;
	m_WaterModel = 0;

	//texture render
	m_ReflectionTexture = 0;
	m_RefractionTexture = 0;

	//shaders
	m_LightShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
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

	/////////////////
	///base stuffs
	/////////////////
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.5f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	/////////////////
	/////models
	/////////////////
	m_GroundModel = new ModelClass;
	if (!m_GroundModel)
	{
		return false;
	}

	result = m_GroundModel->Initialize(m_D3D->GetDevice(), "data/ground.txt",
		L"data/ground01.dds", L"data/ground01.dds", L"data/ground01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	m_WallModel = new ModelClass;
	if (!m_WallModel)
	{
		return false;
	}

	result = m_WallModel->Initialize(m_D3D->GetDevice(), "data/wall.txt",
		L"data/wall01.dds", L"data/wall01.dds", L"data/wall01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
		return false;
	}

	m_BathModel = new ModelClass;
	if (!m_BathModel)
	{
		return false;
	}

	result = m_BathModel->Initialize(m_D3D->GetDevice(), "data/bath.txt",
		L"data/marble01.dds", L"data/marble01.dds", L"data/marble01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bath model object.", L"Error", MB_OK);
		return false;
	}

	m_WaterModel = new ModelClass;
	if (!m_WaterModel)
	{
		return false;
	}

	result = m_WaterModel->Initialize(m_D3D->GetDevice(), "data/water.txt",
		L"data/water01.dds", L"data/water01.dds", L"data/water01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water model object.", L"Error", MB_OK);
		return false;
	}

	/////////////////
	//texture render
	/////////////////
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	/////////////////
	//shaders
	/////////////////
	m_LightShader = new LightShaderClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	m_waterHeight = 2.5f;
	m_waterTranslation = 0.0f;

	return true;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, float rotationY)
{

	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	m_Camera->SetPosition(0.0f, 6.0f, -5.0f);
	m_Camera->SetRotation(rotationY, 0.0f, 0.0f);

	return true;
}

bool GraphicsClass::Render()
{
	bool result = true;

	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	result = RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	result = RenderScene();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderRefractionToTexture()
{
	bool result;

	D3DXVECTOR4 clipPlane;
	D3DXMATRIX world, view, proj;

	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	m_RefractionTexture->
		SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	m_RefractionTexture->
		ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();
	m_D3D->GetWorldMatrix(world);
	m_Camera->GetViewMatrix(view);
	m_D3D->GetProjectionMatrix(proj);

	D3DXMatrixTranslation(&world, 0.0f, 2.0f, 0.0f);

	m_BathModel->Render(m_D3D->GetDeviceContext());

	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(),
		m_BathModel->GetIndexCount(),
		world, view, proj,
		m_BathModel->GetTextureArray()[0],
		m_Light->GetDirection(),
		m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),
		clipPlane
		);
	if (!result)
	{
		return false;
	}

	m_D3D->SetBackBufferRenderTarget();
	return true;
}

bool GraphicsClass::RenderReflectionToTexture()
{
	bool result;

	D3DXMATRIX reflectionView, world, proj;

	m_ReflectionTexture->
		SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	m_ReflectionTexture->
		ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->RenderReflection(m_waterHeight);
	reflectionView = m_Camera->GetReflectionViewMatrix();

	m_D3D->GetWorldMatrix(world);
	m_D3D->GetProjectionMatrix(proj);

	D3DXMatrixTranslation(&world, 0.0f, -6.0f, 8.0f);

	m_WallModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(),
		m_WallModel->GetIndexCount(),
		world, reflectionView, proj,
		m_WallModel->GetTextureArray()[0],
		m_Light->GetDirection(),
		m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower()
		);
	if (!result)
	{
		return false;
	}

	m_D3D->SetBackBufferRenderTarget();
	return true;
}

bool GraphicsClass::RenderScene()
{
	bool result;

	D3DXMATRIX world, view, proj, reflection;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();
	m_Camera->GetViewMatrix(view);
	m_D3D->GetProjectionMatrix(proj);

	m_D3D->GetWorldMatrix(world);
	D3DXMatrixTranslation(&world, 0.0f, 1.0f, 0.0f);

	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(),
		m_GroundModel->GetIndexCount(),
		world, view, proj,
		m_GroundModel->GetTextureArray()[0],
		m_Light->GetDirection(),
		m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower()
		);
	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(world);
	D3DXMatrixTranslation(&world, 0.0f, 6.0f, 8.0f);

	m_WallModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(),
		m_WallModel->GetIndexCount(),
		world, view, proj,
		m_WallModel->GetTextureArray()[0],
		m_Light->GetDirection(),
		m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower()
		);
	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(world);
	D3DXMatrixTranslation(&world, 0.0f, 2.0f, 0.0f);
	m_BathModel->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(),
		m_BathModel->GetIndexCount(),
		world, view, proj,
		m_BathModel->GetTextureArray()[0],
		m_Light->GetDirection(),
		m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower()
		);
	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(world);
	reflection = m_Camera->GetReflectionViewMatrix();
	D3DXMatrixTranslation(&world, 0.0f, m_waterHeight, 0.0f);

	m_WaterModel->Render(m_D3D->GetDeviceContext());

	result = m_WaterShader->Render(m_D3D->GetDeviceContext(),
		m_WaterModel->GetIndexCount(),
		world, view, proj, reflection,
		m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(),
		m_WaterModel->GetTextureArray()[0],
		m_waterTranslation,
		0.01f
		);
	if (!result)
	{
		return false;
	}

	m_D3D->EndScene();

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_WaterModel)
	{
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	if (m_BathModel)
	{
		m_BathModel->Shutdown();
		delete m_BathModel;
		m_BathModel = 0;
	}

	if (m_WallModel)
	{
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = 0;
	}

	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}
