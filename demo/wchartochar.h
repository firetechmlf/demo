#pragma once

#include <iostream>
#include "afxdialogex.h"
using namespace std;

class CUser
{
public:
	CUser();
	virtual~CUser();
	char* WcharToChar(wchar_t* wc);
	wchar_t* CharToWchar(char* c);
	void Release();
	BYTE *CharToBYTE(const char *c);
private:
	char* m_char;
	wchar_t* m_wchar;
	BYTE *m_byte;

};