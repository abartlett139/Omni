#include "GameWorld.h"
#include "GameTimer.h"
#include <time.h>
#include "Graphics.h"
// Resolve linker error for dxerr.lib, which is an old library compiled in vs2012
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

//Hi Amy, how is it going?

Graphics graphics;
bool g_init = false;
GameTimer timer;
SoundEngine *soundEngine = new SoundEngine();

//	WinProc
LRESULT CALLBACK D3D::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		switch (wParam) 
		{
		case SIZE_MAXIMIZED:
			if (!(graphics.d3dpp.Windowed))
				break;
			graphics.d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
			graphics.d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
			graphics.SetScreenRect();
			graphics.d3dpp.Windowed = false;
			break;
		case SIZE_RESTORED:
			//	set windowed to true
			graphics.d3dpp.BackBufferWidth = screenWidth;
			graphics.d3dpp.BackBufferHeight = screenHeight;
			graphics.SetScreenRect();
			graphics.d3dpp.Windowed = true;
			break;
		}break;
	case WM_SETCURSOR:
		SetCursor(NULL);
		graphics.GetDevice()->ShowCursor(true);
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_KEYUP:
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		if(g_init)
			graphics.RecvMessages(msg, wParam, lParam, NULL);
		return 0;
	default:
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------//
//	WinMain
//-----------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstances, LPSTR cmdLine, int showCmd) {

	// cast the state types from generic GameState to their true state
	// reinterpret_cast<GameWorld&>(gameWorld);
	srand(time(NULL));

	// graphics.Initialized(768, 1366, hInstance);
	g_init = graphics.Initialized(screenHeight, screenWidth, hInstance);

	//	initialize the sound engine
	soundEngine->Initialize();

	//	message for loop
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && !IsDialogMessage(NULL, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			timer.Tick();

			graphics.Render();
		}

	}

	return msg.wParam;
}