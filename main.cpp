#include "GameWorld.h"
#include "GameTimer.h"
#include "Input.h"
#include <time.h>

IDirect3DDevice9 *Device;
HWND hwnd;

GameState *currentState, *previousState;
GameState *gameWorld = new GameWorld();

GameTimer timer;

SoundEngine *soundEngine = new SoundEngine();

Input input;

//	WinProc
LRESULT CALLBACK D3D::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
		input.GetKeyPress(wParam);
	case WM_MOUSEMOVE:
		input.UpdateMousePosition(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
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
	reinterpret_cast<GameWorld&>(gameWorld);


	//	set the initial state
	currentState = gameWorld;


	//	initialize the sound engine
	soundEngine->Initialize();

	//	initialize the window
	D3D::InitWindow(hInstance);
	ShowWindow(hwnd, SW_SHOW);

	//	initialize the directx stuff
	D3D::InitD3D();

	//	message for loop
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//	enter the current state
	currentState->Enter();
	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && !IsDialogMessage(NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			timer.Tick();

			currentState->GameLoop();

			srand(time(NULL));

		}



	}

	//	get rid of all extern variables
	D3D::Release<IDirect3DDevice9*>(Device);
	D3D::Delete(gameWorld);



	return msg.wParam;
}