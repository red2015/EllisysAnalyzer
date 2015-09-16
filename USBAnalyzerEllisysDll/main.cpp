#include "stdafx.h"
#include "UsbAnalysis.h"
#include "FrameDecomposer.h"
#include "UsbAnalyzerExplorer200.h"
#include "AnalyzerErrorNotifiacation.h"
#include "SelectAndCreateAnalyzer.h"
#include "Acquisition.h"
#include "Displayers.h"
#include <conio.h>
#include <windows.h> 
#include <extcode.h>
#include <iostream>
#include <ctime>
#include <time.h>
using namespace usbdk;
using namespace std;

#define TEN_MILLISECUND 10

std::wstring g_serialNumber;
RefCountPtr<IUsbAnalyzer> spAnalyzer;
bool g_stopAcqusition = false;
bool g_analyzerErrorOccured = false;
bool g_overflowOccured = false;

BYTE g_frameIn[max_frame_bytecount];
BYTE g_frameOut[max_frame_bytecount];
BYTE g_frameNak[max_frame_bytecount];

unsigned long g_packetsTokenIn = 0;
unsigned long g_packetsTokenOut = 0;
unsigned long g_packetsTokenSetup = 0;
unsigned long g_packetsTokenSOF = 0;
unsigned long g_packetsData0 = 0;
unsigned long g_packetsData1 = 0;
unsigned long g_packetsHandshakeACK = 0;
unsigned long g_packetsHandshakeNAK = 0;

unsigned long g_devicesPackets[128];
int g_devices[128];
int g_addr = 0;
int g_countErrorOverflow = 0;


void AnalyzerErrorNotification(usb_analyzer_error error, usb_analyzer_error_notification_param param, const void* param2)
{
	static const TCHAR* s_analyzerErrorMessage[] =
	{
		_T("No error occured."),
		_T("An overflow occured before all data was read. Please read the user manual for more information"),
		_T("Analyzer and device connected on the same host controller. Please read the user manual for more information"),
		_T("A fatal error occured with the analyzer"),
		_T("A prohibited operation was attempted"),
		_T("Not enough memory to continue the recording process"),
		_T("The analyzer do not support the USB Analysis SDK"),
		NULL,
	};
	TCHAR szBuffer[600];

	g_analyzerErrorOccured = true;

	if(error == 1)
	{
		g_overflowOccured = true;
		g_countErrorOverflow++;
	}

	if(param2 != NULL)
	{
		if(s_analyzerErrorMessage[error] != NULL)
		{
			_stprintf_s(szBuffer, countof(szBuffer), _T("%s (%s)"), s_analyzerErrorMessage[error], param2);
			_tprintf(szBuffer);
		}
		else
		{
			_stprintf_s(szBuffer, countof(szBuffer), _T("%s"), param2);
			_tprintf(szBuffer);
		}
	}
	else
	{
		_stprintf_s(szBuffer, countof(szBuffer), _T("%s."), s_analyzerErrorMessage[error]);
		_tprintf(szBuffer);
	}
}
IUsbAnalyzer* SelectAndCreateAnalyzer(UsbAnalyzerFactoryManager* pAnalyzerFactoryManager, const tstring& desiredSerialNumber)
{
	vector_usb_analyzer_information analyzerInfo;

	// Enumerate all connected analyzers
	pAnalyzerFactoryManager->EnumerateAnalyzers(analyzerInfo);

	// Display an error message if no analyzers were found
	if(analyzerInfo.empty())
	{
		tcout << _T("Error: No USB analyzer found!") << endl;
		return NULL;
	}

	// Display all available USB analyzers
	DisplayAvailableUsbAnalyzers(analyzerInfo);

	// Choose the right serial number among all available USB analyzers
	for(size_t i=0; i<analyzerInfo.size(); i++)
	{
		const UsbAnalyzerInformation& selectedAnalyzer = analyzerInfo[i];

		if(desiredSerialNumber == selectedAnalyzer.GetSerialNumber())
		{
			tcout << _T("Info: Analyzer with serial number ") << desiredSerialNumber << _T(" will be used.") << endl << endl;

			// Found an analyzer with the desired serial number
			return pAnalyzerFactoryManager->CreateAnalyzer(
				selectedAnalyzer.GetFactoryUniqueIdentifier(), 
				selectedAnalyzer.GetUniqueIdentifier());
		}
	}

	// Not found. Display an error message
	if(!desiredSerialNumber.empty())
	{
		tcout << _T("Warning: Serial number ") << desiredSerialNumber << _T(" not found, first analyzer will be used by default!") << endl << endl;
	}

	// Since we have at least one analyzer available, choose it by default in last resort
	const UsbAnalyzerInformation& defaultAnalyzer = analyzerInfo[0];

	return pAnalyzerFactoryManager->CreateAnalyzer(
		defaultAnalyzer.GetFactoryUniqueIdentifier(), 
		defaultAnalyzer.GetUniqueIdentifier());
}
void DoAcquisition(IUsbAnalyzer* pAnalyzer)
{
	//
	// Prepare chainable element sinks
	//
	UsbFrameDecomposer m_frameDecomposer;
	ChainableUsbElementSinkManager sinkChainer;
	g_stopAcqusition = false;
	g_analyzerErrorOccured = false;
	g_overflowOccured = false;
	
	m_frameDecomposer.SetAllTrasactions(g_packetsTokenIn, g_packetsTokenOut,g_packetsTokenSetup, g_packetsTokenSOF, g_packetsData0, g_packetsData1, g_packetsHandshakeNAK, g_packetsHandshakeACK);
	m_frameDecomposer.SetDevicesPackets(g_devicesPackets, g_devices);
	sinkChainer.AddElementSink(&m_frameDecomposer);
	_tprintf(_T("\n"));

	//
	// Do Acquisition
	//
	_tprintf(_T("Do Acquisition\n"));
	_tprintf(_T("--------------\n"));

	try
	{
		pAnalyzer->BeginAcquisition(&sinkChainer, &AnalyzerErrorNotification);
	}
	catch(std::tstring& s)
	{
		_tprintf(_T("\n%s\n"), s.c_str());
	}
	catch(std::exception& e)
	{
		_tprintf(_T("\nException caught!\n%s\n"), e.what());
	}
	int inc = 0;
	clock_t actualTime, oldTime = clock();
	printf("\nZapylam");
	// Type a key to stop the acquisition...
	for(;;)
	{
		if(_kbhit())
		{
			_getch();
			break;
		}
	
		if(g_analyzerErrorOccured || g_stopAcqusition)
		{
			break;
		}
		actualTime = clock();
		
		if(long(actualTime-oldTime) > TEN_MILLISECUND)
		{
			m_frameDecomposer.DecreaseAll();
			oldTime = actualTime;
		}
		m_frameDecomposer.GetFrame(g_frameIn, g_frameOut, g_frameNak);
		g_packetsTokenIn = m_frameDecomposer.GetCountTransactionsIn();
		g_packetsTokenOut = m_frameDecomposer.GetCountTransactionsOut();
		g_packetsTokenSetup = m_frameDecomposer.GetCountTransactionsTokenSetup();
		g_packetsTokenSOF = m_frameDecomposer.GetCountTransactionsTokenSOF();
		g_packetsData0 = m_frameDecomposer.GetCountTransactionsData0();
		g_packetsData1 = m_frameDecomposer.GetCountTransactionsData1();
		g_packetsHandshakeACK = m_frameDecomposer.GetCountTransactionsACK();
		g_packetsHandshakeNAK = m_frameDecomposer.GetCountTransactionsNak();
		g_addr = m_frameDecomposer.GetDevicesPackets(g_devicesPackets, g_devices);
	}

	pAnalyzer->EndAcquisition();
}


extern "C" int _declspec(dllexport)
GetAnalyzerSerialNumber(size_t *sizeOut,char *myString)
{
	char *out;
	size_t lenSerialNumber = 0;
	lenSerialNumber = wcslen(g_serialNumber.c_str());
	out = (char*) malloc(sizeof(char)*lenSerialNumber);
	wcstombs(out, g_serialNumber.c_str(), lenSerialNumber);
	*sizeOut = lenSerialNumber;
	memcpy(myString, out, lenSerialNumber);
	return 0;
}

extern "C" int _declspec(dllexport)
FindAnalyzer()
{
	
	UsbAnalyzerFactoryManager analyzerFactoryManager;
	UsbExplorer200_RegisterAnalyzerFactory(&analyzerFactoryManager);
	spAnalyzer = SelectAndCreateAnalyzer(&analyzerFactoryManager, g_serialNumber);
	if(spAnalyzer == NULL)
	{
		return -1;
	}
	g_serialNumber = spAnalyzer->GetSerialNumber().c_str();
	return 0;
}

extern "C" int _declspec(dllexport)
Acqusiton()
{
	if(spAnalyzer == NULL)
	{
		return -1;
	}	
	do
	{
		DoAcquisition(spAnalyzer);
	}while(g_overflowOccured == true);
	return 0;
}


extern "C" int _declspec(dllexport)
GetFrameStatistics(uint8_t *frameIn[], uint8_t *frameOut[], uint8_t *frameNak[], size_t *sizeFrameIn, size_t *sizeFrameOut, size_t *sizeFrameNak)
{		
	*sizeFrameIn = sizeof(unsigned char)* max_frame_bytecount;
	*sizeFrameOut = sizeof(unsigned char)* max_frame_bytecount;
	*sizeFrameNak = sizeof(unsigned char)* max_frame_bytecount;
	memcpy(frameIn, g_frameIn, max_frame_bytecount);
	memcpy(frameOut, g_frameOut, max_frame_bytecount);
	memcpy(frameNak, g_frameNak, max_frame_bytecount);
	return 0;
}

extern "C" int _declspec(dllexport)
StopAcqusiton()
{
	g_stopAcqusition = true;
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountOfTransactionInOutNak(unsigned long *in, unsigned long *out, unsigned long *nak)
{
	*in = g_packetsTokenIn;
	*out = g_packetsTokenOut;
	*nak = g_packetsHandshakeNAK;
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountAllTransactions(unsigned long *in, unsigned long  *out, unsigned long  *setup, unsigned long  *sof, unsigned long  *data0, unsigned long  *data1, unsigned long  *ack, unsigned long  *nak)
{
	*in = g_packetsTokenIn;
	*out = g_packetsTokenOut;
	*setup = g_packetsTokenSetup;
	*sof = g_packetsTokenSOF;
	*data0 = g_packetsData0;
	*data1 = g_packetsData1;
	*ack = g_packetsHandshakeACK;
	*nak = g_packetsHandshakeNAK;
	return 0;
}

extern "C" int _declspec(dllexport)
GetDeviceTransactions(unsigned  long *transactions,int *devices, int *size, unsigned  long *trans_, int* dev_)
{
	
	unsigned long *transactionsLocalCopy = g_devicesPackets;
	int *devicesLocalCopy = g_devices;
	unsigned long trans[128];
	int dev[128];
	for(int k = 0; k < 128; k++)
	{
		trans[k] = 0;
		dev[k] = 0;
	}
	int j =0;
	for(int i = 0; i < 128; i++)
	{
		if(devicesLocalCopy[i] > 0)
		{
			trans[j] = transactionsLocalCopy[i];
			dev[j] = i;
			j++;
		}
	}
	memcpy(transactions, transactionsLocalCopy, 128);
	memcpy(devices, devicesLocalCopy, 128);
	memcpy(trans_,trans, 128);
	memcpy(dev_,dev, 128);
	*size = j;
	return g_addr;
}

extern "C" int _declspec(dllexport)
ResetAll()
{
	g_stopAcqusition = true;
	for (int i = 0; i < 128; i++)
	{
		g_devices[i]= 0;
		g_devicesPackets[i] = 0;
	}
	g_packetsTokenIn = 0;
	g_packetsTokenOut = 0;
	g_packetsTokenSetup = 0;
	g_packetsTokenSOF = 0;
	g_packetsData0 = 0;
	g_packetsData1 = 0;
	g_packetsHandshakeACK = 0;
	g_packetsHandshakeNAK = 0;
	for(int i =0; i < max_frame_bytecount; i++)
	{
		g_frameIn[i] = 0;
		g_frameOut[i] = 0;
		g_frameOut[i] = 0;
	}
	g_stopAcqusition = false;
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountErrorOverflow(int *count)
{
	*count = g_countErrorOverflow;
	return 0;
}