#include "..\Shared\Displayers.h"
#include "..\Shared\Filters.h"
#include "..\Shared\Statistics.h"
#include <conio.h>

using namespace usbdk;
using namespace std;

bool stopAcquisition = false;
UsbFrameDecomposer m_frameDecomposer;

void DoAcquisition(IUsbAnalyzer* pAnalyzer)
{
	//
	// Prepare chainable element sinks
	//
	ChainableUsbElementSinkManager sinkChainer;


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

	// Type a key to stop the acquisition...
	for(;;)
	{
		if(_kbhit())
		{
			_getch();
			break;
		}

		if(g_analyzerErrorOccured || stopAcquisition)
		{
			break;
		}
	}

	pAnalyzer->EndAcquisition();
}