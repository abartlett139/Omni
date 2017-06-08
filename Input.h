#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <windowsx.h>

class Input
{
public:
	Input();
	~Input();

	//	void Initialize();
	//	void Update();

	void GetKeyPress(WPARAM wParam) {
		
	}

	void UpdateMousePosition(LPARAM lParam) {
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
	}



	float mouseX, mouseY;

};

#endif