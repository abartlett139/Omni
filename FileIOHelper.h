#pragma once
#ifndef FILEIOHELPER_H
#define FILEIOHELPER_H
#include <d3dx9.h>
#include <iostream>
#include <fstream>  
#include <ostream>
#include <string>
#include <vector>
class FileIOHelper
{
    std::string m_filename;
	std::vector<std::string> m_text;
	ID3DXFont * m_font;
	IDirect3DDevice9 * m_dev;
	RECT m_textRect;
	DWORD m_format;
public:
    FileIOHelper(IDirect3DDevice9 * Device, std::string filename, RECT rect);
	FileIOHelper(IDirect3DDevice9 * Device, std::string filename,RECT rect, LPSTR fontName,
		int height, int width,UINT weight, DWORD format);

    ~FileIOHelper();
	void Enter();
	void Exit();
	void getRecord(std::string recordName);
	void Render(LPD3DXSPRITE sprite);
};
#endif // !FILEIOHELPER_H
