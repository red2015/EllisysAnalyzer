using namespace usbdk;
using namespace std;

bool g_analyzerErrorOccured = false;

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