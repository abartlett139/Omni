#include "FileIOHelper.h"


FileIOHelper::FileIOHelper(IDirect3DDevice9 * Device, std::string filename, RECT rect) :
	m_dev(Device), m_filename(filename), m_textRect(rect)
{
	m_format |= DT_LEFT|DT_TOP;
	D3DXCreateFont(m_dev, 50, 15, FW_BOLD, 0, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"),
		&m_font);
}

FileIOHelper::FileIOHelper(IDirect3DDevice9 * Device, std::string filename, RECT rect, 
	LPSTR fontName, int height, int width, UINT weight, DWORD format):
	m_dev(Device), m_filename(filename), m_textRect(rect), m_format(format)

{
	D3DXCreateFont(m_dev, height, width, weight, 0, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, TEXT(fontName),
		&m_font);
}

FileIOHelper::~FileIOHelper()
{
}

void FileIOHelper::Enter()
{

}

void FileIOHelper::Exit()
{
}

void FileIOHelper::getRecord(std::string recordName)
{
	if (recordName != "")
	{
		std::ifstream l_finOut(m_filename);
		l_finOut.seekg(0);

		if (!l_finOut)
		{
			MessageBox(NULL, "Cannot open menu file", NULL, NULL);
		}
		while (!l_finOut.eof())
		{
			std::string temp;
			std::getline(l_finOut, temp);
			if (temp == recordName)
			{
				while (temp[0] != '0')
				{
					std::getline(l_finOut, temp);
					m_text.push_back(temp);
				}
				l_finOut.clear();
				l_finOut.close();
				return;//end of record has been found
			}
		}
	}
}

void FileIOHelper::Render(LPD3DXSPRITE sprite)
{
	if (m_filename != "")
	{
		//ZeroMemory(&m_textRect, sizeof(RECT));
		RECT temp = m_textRect;
	
		for (unsigned int i = 0; i < m_text.size(); i++)
		{
			m_font->DrawText(NULL, m_text[i].c_str(), -1, &temp, m_format, D3DCOLOR_XRGB( 255, 255, 0 ) );
			//D3DXMatrixTranslation(&textPos, m_textRect.left, m_textRect.bottom, 0);
			temp.top += 20;
			temp.bottom += 20;
		}
	}
}
