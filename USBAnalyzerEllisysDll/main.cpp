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
#include <iostream>
using namespace usbdk;
using namespace std;

std::wstring serialNumber;
RefCountPtr<IUsbAnalyzer> spAnalyzer;
bool stop = false;
UsbFrameDecomposer m_frameDecomposer1;
bool g_analyzerErrorOccured = false;
bool acqusition = false;
BYTE frameIn1[max_frame_bytecount];
BYTE frameOut1[max_frame_bytecount];
BYTE frameNak1[max_frame_bytecount];


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
	stop = false;

	sinkChainer.AddElementSink(&m_frameDecomposer1);
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
		m_frameDecomposer1.GetFrame(frameIn1, frameOut1, frameNak1);
		if(_kbhit())
		{
			_getch();
			break;
		}

		if(g_analyzerErrorOccured || stop)
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
	lenSerialNumber = wcslen(serialNumber.c_str());
	out = (char*) malloc(sizeof(char)*lenSerialNumber);
	wcstombs(out, serialNumber.c_str(), lenSerialNumber);
	*sizeOut = lenSerialNumber;
	memcpy(myString, out, lenSerialNumber);
	return 0;
}

extern "C" int _declspec(dllexport)
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

extern "C" int _declspec(dllexport)
Acqusiton()
{
		if(spAnalyzer == NULL)
		{
			return -1;
		}
		DoAcquisition(spAnalyzer);
		acqusition = true;
		
		if(g_analyzerErrorOccured == true)
		{
			return -1;
		}
		return 0;
}


extern "C" int _declspec(dllexport)
GetFrameStatistics(uint8_t *frameIn[], uint8_t *frameOut[], uint8_t *frameNak[], size_t *sizeFrameIn, size_t *sizeFrameOut, size_t *sizeFrameNak)
{
			
		*sizeFrameIn = sizeof(unsigned char)* max_frame_bytecount;
		*sizeFrameOut = sizeof(unsigned char)* max_frame_bytecount;
		*sizeFrameNak = sizeof(unsigned char)* max_frame_bytecount;
		m_frameDecomposer1.GetFrame(frameIn1, frameOut1, frameNak1);
		memcpy(frameIn, frameIn1, max_frame_bytecount);
		memcpy(frameOut, frameOut1, max_frame_bytecount);
		memcpy(frameNak, frameNak1, max_frame_bytecount);
		return frameIn1[33];
}

extern "C" int _declspec(dllexport)
DecreaseFrame()
{
	Sleep(10);
	m_frameDecomposer1.DecreaseAll();
	return 0;
}

extern "C" int _declspec(dllexport)
StopAcqusiton()
{
	stop = true;
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountOfTransactionInOutNak(unsigned long *in, unsigned long *out, unsigned long *nak)
{
	*in = m_frameDecomposer1.GetCountTransactionsIn();
	*out = m_frameDecomposer1.GetCountTransactionsOut();
	*nak = m_frameDecomposer1.GetCountTransactionsNak();
	return 0;
}

extern "C" int _declspec(dllexport)
GetCountAllTransactions(unsigned long *in, unsigned long  *out, unsigned long  *setup, unsigned long  *sof, unsigned long  *data0, unsigned long  *data1, unsigned long  *ack, unsigned long  *nak)
{
	*in = m_frameDecomposer1.GetCountTransactionsIn();
	*out = m_frameDecomposer1.GetCountTransactionsOut();
	*setup = m_frameDecomposer1.GetCountTransactionsTokenSetup();
	*sof = m_frameDecomposer1.GetCountTransactionsTokenSOF();
	*data0 = m_frameDecomposer1.GetCountTransactionsData0();
	*data1 = m_frameDecomposer1.GetCountTransactionsData1();
	*ack = m_frameDecomposer1.GetCountTransactionsACK();
	*nak = m_frameDecomposer1.GetCountTransactionsNak();
	return 0;
}

extern "C" int _declspec(dllexport)
ZwrocTabChar(uint8_t *tabchar, size_t *sizeOut)
{
	unsigned char tab[5];
	tab[0] = 'A';
	tab[1] = 'B';
	tab[2] = 'C';
	tab[3] = 'D';
	tab[4] = 'E';
	*sizeOut = 5;
	//tabchar = (uint8_t*) malloc(sizeof(uint8_t)*5);
	memcpy(tabchar, tab, 5);
	return tab[2];
}

extern "C" int _declspec(dllexport)
ZwrocChar(unsigned char char_)
{
	char_ = 1;
	return char_;
}

