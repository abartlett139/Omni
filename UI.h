#ifndef UI_H_
#define UI_H_
#include <d3dx9.h>
#include <string>

class UIDevice
{
public:
	enum drawType { centered, left, right };
private:
	ID3DXFont * font;
	IDirect3DDevice9 * dev;
public:
	UIDevice(IDirect3DDevice9 * Dev);
	~UIDevice();

	void Draw(std::string, D3DXVECTOR2 pos, drawType dt);

};

#endif
