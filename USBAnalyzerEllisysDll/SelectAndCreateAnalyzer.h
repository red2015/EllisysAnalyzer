#include "..\Shared\Displayers.h"

using namespace usbdk;
using namespace std;

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