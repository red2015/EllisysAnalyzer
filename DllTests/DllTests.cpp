// DllTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <windows.h> 
#include "stdafx.h"
#include <cstdlib> //added http://forums.devx.com/showthread.php?t=152439
#include <string>
#include <iostream>
#include <fstream>

typedef unsigned char   uint8_t;
using namespace std;

extern "C" __declspec(dllimport)int ZwrocTabChar(uint8_t *tabchar, size_t *sizeOut);
extern "C" __declspec(dllimport)int FindAnalyzer();
extern "C" __declspec(dllimport)int Acqusiton();

int _tmain(int argc, _TCHAR* argv[])
{
	FindAnalyzer();
	Acqusiton();
	return 0;
}

