#ifndef D3D_H
#define D3D_H

#include <d3dx9.h>
#include <windows.h>


extern IDirect3DDevice9 *Device;
extern HWND hwnd;
static D3DPRESENT_PARAMETERS d3dpp;

#define screenWidth 800
#define screenHeight 600

namespace D3D {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool InitWindow(HINSTANCE hInstance);
	bool InitD3D();
	
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));


	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color);


	float Lerp(float a, float b, float t);


	//	vertex struct for environment objects
	struct EVertex {
		EVertex() {}
		EVertex(float X, float Y, float Z, float U, float V) : x(X), y(Y), z(Z), u(U), v(V) {}
		float x, y, z, u, v;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};


	//	bounding box structure for collision detection and particle systems
	struct BoundingBox {

		BoundingBox();

		//	compares point to bounding vectors
		bool isPointInside(D3DXVECTOR3 &point);
		//	collision between bounding boxes
		bool isCollision(BoundingBox &box);

		D3DXVECTOR3 _min, _max;
	};


	//	overloaded operators for comparing vectors
	bool operator<=(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b);
	bool operator>=(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b);


	//	distance checking function for 3D vectors
	float getDistance(D3DXVECTOR3 &a, D3DXVECTOR3 &b);


	//	release method
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	//	delete method
	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}
}

#endif