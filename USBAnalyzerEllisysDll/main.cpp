#include "stdafx.h"
#include "UsbAnalysis.h"
#include "FrameDecomposer.h"
#include "UsbAnalyzerExplorer200.h"
#include "AnalyzerErrorNotifiacation.h"
#include "SelectAndCreateAnalyzer.h"
#include "Acquisition.h"
#include <conio.h>
#include <windows.h> 
#include <extcode.h>
using namespace usbdk;
using namespace std;

std::wstring serialNumber;
RefCountPtr<IUsbAnalyzer> spAnalyzer;

int32_t _declspec(dllexport)
GetAnalyzerSerialNumber(size_t *sizeOut,char *myString)
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
	spAnalyzer = SelectAndCreateAnalyzer(&analyzerFactoryManager, serialNumber);
	if(spAnalyzer == NULL)
	{
		return -1;
	}
	serialNumber = spAnalyzer->GetSerialNumber().c_str();
	return 0;
}

int32_t _declspec(dllexport)
Acqusiton()
{
	if(spAnalyzer == NULL)
	{
		return -1;
	}
	DoAcquisition(spAnalyzer);
	if(g_analyzerErrorOccured == true)
	{
		return -1;
	}
	return 0;
}

int32_t _declspec(dllexport)
GetFrameStatistics(BYTE *frameIn, BYTE *frameOut, BYTE *frameNak, size_t *sizeFrameIn, size_t *sizeFrameOut, size_t *sizeFrameNak)
{
	frameIn = new BYTE[max_frame_bytecount];
	frameOut = new BYTE[max_frame_bytecount];
	frameNak = new BYTE[max_frame_bytecount];
	*sizeFrameIn = max_frame_bytecount;
	*sizeFrameOut = max_frame_bytecount;
	*sizeFrameNak = max_frame_bytecount;
	m_frameDecomposer.GetFrame(frameIn, frameOut, frameNak);
	return 0;
}

int32_t _declspec(dllexport)
StopAcqusiton()
{
	stopAcquisition = true;
	return 0;
}

