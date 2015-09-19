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
using namespace usbdk;
using namespace std;

#define TEN_MILLISECUND 10
UsbFrameDecomposer g_frameDecomposer;
std::wstring g_serialNumber;
RefCountPtr<IUsbAnalyzer> spAnalyzer;
bool g_stopAcqusition = false;
bool g_analyzerErrorOccured = false;
bool g_overflowOccured = false;
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
	ChainableUsbElementSinkManager sinkChainer;
	g_stopAcqusition = false;
	g_analyzerErrorOccured = false;
	g_overflowOccured = false;
	sinkChainer.AddElementSink(&g_frameDecomposer);
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
GetFrameStatistics(uint8_t *frameIn, uint8_t *frameOut, uint8_t *frameNak, size_t *sizeFrameIn, size_t *sizeFrameOut, size_t *sizeFrameNak)
{		
	*sizeFrameIn = sizeof(unsigned char)* max_frame_bytecount;
	*sizeFrameOut = sizeof(unsigned char)* max_frame_bytecount;
	*sizeFrameNak = sizeof(unsigned char)* max_frame_bytecount;
	g_frameDecomposer.GetFrame(frameIn,frameOut,frameNak);
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
	*in = g_frameDecomposer.GetCountTransactionsIn();
	*out = g_frameDecomposer.GetCountTransactionsOut();
	*nak = g_frameDecomposer.GetCountTransactionsNak();
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountAllTransactions(unsigned long *in, unsigned long  *out, unsigned long  *setup, unsigned long  *sof, unsigned long  *data0, unsigned long  *data1, unsigned long  *ack, unsigned long  *nak)
{
	*in = g_frameDecomposer.GetCountTransactionsIn();
	*out = g_frameDecomposer.GetCountTransactionsOut();
	*setup = g_frameDecomposer.GetCountTransactionsTokenSetup();
	*sof = g_frameDecomposer.GetCountTransactionsTokenSOF();
	*data0 = g_frameDecomposer.GetCountTransactionsData0();
	*data1 = g_frameDecomposer.GetCountTransactionsData1();
	*ack = g_frameDecomposer.GetCountTransactionsACK();
	*nak = g_frameDecomposer.GetCountTransactionsNak();
	return 0;
}

extern "C" int _declspec(dllexport)
GetDeviceTransactions(unsigned  long *transactions,int *devices, int *size, unsigned  long *trans_, int* dev_)
{
	
	unsigned long transactionsLocalCopy[128];
	int devicesLocalCopy[128];
	g_frameDecomposer.GetDevicesPackets(transactionsLocalCopy,devicesLocalCopy);
	unsigned long trans[128];
	int dev[128];
	for(int k = 0; k < 128; k++)
	{
		trans[k] = 0;
		dev[k] = 0;
	}
	int j =0;
	for(int i = 1; i < 128; i++)
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
	return 0;
}
extern "C" int _declspec(dllexport)
GetCountErrorOverflow(int *count)
{
	*count = g_countErrorOverflow;
	return 0;
}

extern "C" int _declspec(dllexport)
DecreaseAll()
{
	g_frameDecomposer.DecreaseAll();
	return 0;
}