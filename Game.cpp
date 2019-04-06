#include "Game.h"


//Variables
IDirect3DDevice9			*g_Device = NULL;

HINSTANCE					 g_hInstance = NULL;

HWND						 g_hwnd = NULL;


bool						 g_LMBDown = FALSE;				//Mouse's left

int							 g_MouseX = 0, g_MouseY = 0;

D3DXMATRIX					 g_matWorld;

Terrain					    *TheTerrain = 0;

Camera						  TheCamera(Camera::LANDOBJECT);


D3DXMATRIX g_projection;
D3DXMATRIX g_ViewMatrix;


//////////////////////////////////////////////////////////////////////////
//Name:bool Game_Init()
//Describe:Initialize the Game
//////////////////////////////////////////////////////////////////////////
bool Game_Init()
{
	//Initialize the Window
	if (!Init_D3D(D3DDEVTYPE_HAL))
	{
		::MessageBox(NULL, "Initialize Direct3D Failed", "Error", MB_OK);
		return FALSE;
	}

	Setup();
	return true;
}


//////////////////////////////////////////////////////////////////////////
//Name:void Game_Main(float fTimeDelta)
//Describe:the Game's main loop
//////////////////////////////////////////////////////////////////////////
void Game_Main(float fTimeDelta)
{	
	D3D_Update(fTimeDelta);
	D3D_Render(fTimeDelta);
}


//////////////////////////////////////////////////////////////////////////
//Name:void Game_Shutdown()
//Describe:the game is over ,so release the all sources
//////////////////////////////////////////////////////////////////////////
void Game_Shutdown()
{
	SAFE_DELETE(TheTerrain);

	g_Device->Release();
}


//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Update(float fTimeDelta)
//Describe:on-time Update the game's date
//////////////////////////////////////////////////////////////////////////
void D3D_Update(float fTimeDelta)
{
	if (g_Device)
	{
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera.walk(100.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera.walk(-100.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera.yaw(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera.yaw(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera.strafe(-100.0f * fTimeDelta);

		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera.strafe(100.0f * fTimeDelta);

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera.pitch(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			TheCamera.pitch(-1.0f *fTimeDelta);

		D3DXVECTOR3 pos;
		TheCamera.getPosition(&pos);
		float height = TheTerrain->getHeight(pos.x, pos.z);
		pos.y = height + 5.0f; // add height because we're standing up
		TheCamera.setPosition(&pos);

		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		g_Device->SetTransform(D3DTS_VIEW, &V);
	}
}


//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Render(float fTimeDelta)
//Describe:on-time Render the Game
//////////////////////////////////////////////////////////////////////////
void D3D_Render(float fTimeDelta)
{
	//1.Clear the Screen
	g_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);

	//2.Start BeginScene
	g_Device->BeginScene();

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	D3DXMatrixScaling(&I, 0.50f, 0.50f, 0.50f);

	if (TheTerrain)
		TheTerrain->draw(&I, FALSE);				//true = line,false = normal
	

	//3.End Draw

	g_Device->EndScene();

	g_Device->Present(NULL, NULL, NULL, NULL);  // ·­×ªÓëÏÔÊ¾
}


//////////////////////////////////////////////////////////////////////////
// Name:Init_D3D()
// Describe:Initialize D3D window
//////////////////////////////////////////////////////////////////////////
bool Init_D3D(D3DDEVTYPE deviceType)
{
	HWND hwnd;

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = (HICON)LoadImage(0, "Terrain\\icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIconSm = (HICON)LoadImage(0, "Terrain\\icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = WINDOW_TITLE;

	if (!RegisterClassEx(&wc))
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}

	hwnd = ::CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION ,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0 /*parent hwnd*/, 0 /* menu */, g_hInstance, 0 /*extra*/);

	if (!hwnd)
	{
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}
	g_hwnd = hwnd;

	::ShowWindow(hwnd, SW_SHOW);

	::UpdateWindow(hwnd);

	// disable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	//
	// Init D3D: 
	//

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.
	int mode;					//Set the full screen or windowed

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	mode = MessageBoxW(NULL, L"Use FullScreen mode?", L"Tips", MB_YESNO | MB_ICONQUESTION);

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;

	if (mode == IDYES)
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = FALSE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = TRUE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&g_Device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&g_Device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	return true;
}


bool Setup()
{
	//
	// Create the terrain.
	//

	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	TheTerrain = new Terrain(g_Device, "castlehm257.raw", 256, 256, 1,0.25f);
	TheTerrain->genTexture(&lightDirection);

	//
	// Set texture filters.
	//

	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		WINDOW_WIDTH/ WINDOW_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

