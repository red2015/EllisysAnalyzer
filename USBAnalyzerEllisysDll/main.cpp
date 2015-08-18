#include "stdafx.h"
#include "UsbAnalysis.h"
#include "FrameDecomposer.h"
#include "UsbAnalyzerExplorer200.h"
#include "AnalyzerErrorNotifiacation.h"
#include "SelectAndCreateAnalyzer.h"
#include <conio.h>
#include <windows.h> 
#include <extcode.h>
using namespace usbdk;
using namespace std;

std::wstring serialNumber;

int32_t _declspec(dllexport)
GetAnalyzerSerialNumber(size_t *sizeOut,char myString[])
{
	char *out;
	size_t lenSerialNumber = 0;
	lenSerialNumber = wcslen(serialNumber.c_str());
	out = (char*) malloc(sizeof(char)*lenSerialNumber);
	wcstombs(out, serialNumber.c_str(), lenSerialNumber);
	*sizeOut = lenSerialNumber;
	memcpy(myString, out, lenSerialNumber);
	return 0;
}

int32_t _declspec(dllexport)
FindAnalyzer()
{
	UsbAnalyzerFactoryManager analyzerFactoryManager;
	UsbExplorer200_RegisterAnalyzerFactory(&analyzerFactoryManager);
	RefCountPtr<IUsbAnalyzer> spAnalyzer = SelectAndCreateAnalyzer(&analyzerFactoryManager, serialNumber);
	if(spAnalyzer == NULL)
	{
		return -1;
	}
	serialNumber = spAnalyzer->GetSerialNumber().c_str();
	//DoAcquisition(spAnalyzer);
	return 0;
}