#include "stdafx.h"
#include "wchartochar.h"

CUser::CUser() :m_char(NULL), m_wchar(NULL), m_byte(NULL)
{
}

CUser::~CUser()
{
	Release();
}

void CUser::Release()
{
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = NULL;
	}
	if (m_byte)
	{
		delete m_byte;
		m_byte = NULL;
	}
}
char* CUser::WcharToChar(wchar_t* wc)
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

wchar_t* CUser::CharToWchar(char* c)
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
BYTE* CUser::CharToBYTE(const char *c)
{
	
	int nLength = strlen(c);
	int nSize = (nLength / 2) + (nLength % 2);
	int temp;
	static BYTE return_byte[7] = { 0 };
	memset(return_byte, 0, 7);
	static BYTE* byte_buf = (BYTE*)malloc(nSize*sizeof(BYTE));
	memset(byte_buf, 0, nSize*sizeof(BYTE));

	for (temp = 0; temp < nSize; temp++)
	{
		char szTemp[3] = { 0 };
		szTemp[2] = '\0';
		szTemp[0] = *c++;
		if (*c != '\0')
		{
			szTemp[1] = *c++;

		}
		int nNumber = strtol(szTemp,NULL,16);
		byte_buf[temp] = nNumber;

	}
	for (temp = 0; temp < 6; temp++)
	{
		return_byte[temp] = byte_buf[temp*2];
	}
	return return_byte;
}