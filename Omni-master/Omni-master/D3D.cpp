#include "D3D.h"



bool D3D::InitWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)D3D::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "D3DApp";

	if (!RegisterClass(&wc))
		return false;


	hwnd = CreateWindow("D3DApp", "Omni", WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX, 100, 100, screenWidth, screenHeight, 0, 0, hInstance, 0);
	if (!hwnd)
		return false;

	return true;
}

bool D3D::InitD3D()
{
	HRESULT hr = 0;

	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &Device);

	if (FAILED(hr)) {
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &Device);

		if (FAILED(hr)) {
			d3d9->Release();
			return false;
		}
	}

	d3d9->Release();


	return true;
}

D3DMATERIAL9 D3D::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

D3DLIGHT9 D3D::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color;
	light.Diffuse = *color;
	light.Specular = *color;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 D3D::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = *color;
	light.Specular = *color;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.125f;
	light.Attenuation2 = 0.0f;
	light.Phi = D3DXToRadian(40.0f);
	light.Theta = D3DXToRadian(20.0f);
	light.Falloff = 1.0f;

	return light;
}

D3DLIGHT9 D3D::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.5f;
	light.Diffuse = *color * 0.5f;
	light.Specular = *color;
	light.Position = *position;
	light.Range = 100.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.125f;
	light.Attenuation2 = 0.0f;

	return light;
}

float D3D::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

bool D3D::operator<=(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b)
{
	if(a.x <= b.x && a.y <= b.y && a.z <= b.z)
		return true;
	return false;
}

bool D3D::operator>=(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b)
{
	if (a.x >= b.x && a.y >= b.y && a.z >= b.z)
		return true;
	return false;
}

float D3D::getDistance(D3DXVECTOR3 & a, D3DXVECTOR3 & b)
{
	return (float)sqrt(((b.x - a.x)*(b.x - a.x)) + ((b.y - a.y)*(b.y - a.y)) + ((b.z - a.z)*(b.z - a.z)));
}

//	default constructor for bounding box structure -- sets default _min and _max to INFINITY
D3D::BoundingBox::BoundingBox() : _min(-INFINITY, -INFINITY, -INFINITY), _max(INFINITY, INFINITY, INFINITY)
{
}

bool D3D::BoundingBox::isPointInside(D3DXVECTOR3 & point)
{
	//	if the point is less than the max and greater than the min
	if (point <= _max && point >= _min)
		return true;

	return false;
}

bool D3D::BoundingBox::isCollision(BoundingBox & box)
{
	//	check to see if box._min is inside our bounding vectors
	if (isPointInside(box._min))
		return true;
	//	check to see if box._min is inside our bounding vectors
	if (isPointInside(box._max))
		return true;

	return false;
}
