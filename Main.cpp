#ifndef STRICT
#define STRICT
#endif

#define WIN32_LEAN_AND_MEAN

#include "Game.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		return 0;
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		ValidateRect(hwnd, NULL);				//Update the client
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;

	case WM_DESTROY:
	{
		Game_Shutdown();
		::PostQuitMessage(0);
	}
	break;
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;

	//Enable run-time memory check for debug builds
#if defined(DEBUG)|defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	g_hInstance = hInstance;

	if (Game_Init())
	{
		::ZeroMemory(&msg, sizeof(MSG));
		while (true)
		{
			static FLOAT fLastTime = (float)::timeGetTime();
			static FLOAT fCurrTime = (float)::timeGetTime();
			static FLOAT fTimeDelta = 0.0f;
			fCurrTime = (float)::timeGetTime();
			fTimeDelta = (fCurrTime - fLastTime) * 0.001f;
			fLastTime = fCurrTime;

			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				//test if this is a quit
				if (msg.message == WM_QUIT)
				{
					break;
				}
				//translate any accelerator keys
				TranslateMessage(&msg);

				//send the message to the window proc
				DispatchMessageW(&msg);
			}//end if
			else
			{
				Game_Main(fTimeDelta);
			}
		}
	}

	Game_Shutdown();

	UnregisterClass(WINDOW_TITLE, hInstance);
	return 0;
}