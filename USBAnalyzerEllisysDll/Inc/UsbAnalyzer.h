// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbAnalyzer.h
/// @brief
///		USB Analysis SDK analyzers declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

//--------------------------------------------------------------------
// UsbAnalyzerInformation
//--------------------------------------------------------------------

/// @brief
/// 	Stores information about an USB analyzer.
/// @seealso
/// 	IUsbAnalyzerFactory
class UsbAnalyzerInformation
{
private:
	std::tstring m_manufacturer;
	std::tstring m_modelName;
	std::tstring m_serialNumber;
	std::tstring m_uniqueIdentifier;
	GUID m_factoryUniqueIdentifier;

public:
	/// @brief
	/// 	Constructs a new empty UsbAnalyzerInformation object.
	UsbAnalyzerInformation();

	/// @brief
	/// 	Constructs a new empty UsbAnalyzerInformation object.
	/// @param 
	///		manufacturer - The manufacturer of the analyzer.
	/// @param 
	///		modelName - The model name of the analyzer.
	/// @param 
	///		serialNumber - The serial number of the analyzer.
	/// @param 
	///		uniqueIdentifier - The unique identifier of the analyzer.
	/// @param 
	///		factoryUniqueIdentifier - The unique identifier of the analyzer factory.
	UsbAnalyzerInformation(const std::tstring& manufacturer, const std::tstring& modelName, const std::tstring& serialNumber, const std::tstring& uniqueIdentifier, const GUID& factoryUniqueIdentifier);

	/// @brief
	/// 	Constructs a new empty UsbAnalyzerInformation object.
	/// @param
	/// 	copy - The object to copy from.
	UsbAnalyzerInformation(const UsbAnalyzerInformation& copy);

	/// @brief
	/// 	Destroys an UsbAnalyzerInformation object.
	~UsbAnalyzerInformation();

public:
	/// Gets the manufacturer of the analyzer.
	std::tstring GetManufacturer() const;

	/// Gets the model name of the analyzer.
	std::tstring GetModelName() const;

	/// Gets the serial number of the analyzer.
	std::tstring GetSerialNumber() const;

	/// Gets the full name of the analyzer including the manufacturer, the model name and the serial number.
	std::tstring GetFullName() const;

	/// @brief
	/// 	Gets the unique identifier of the analyzer.
	/// @seealso
	/// 	IUsbAnalyzerFactory::CreateAnalyzer
	std::tstring GetUniqueIdentifier() const;

	/// @brief
	/// 	Gets the unique identifier of the analyzer factory.
	/// @seealso
	/// 	IUsbAnalyzerFactory::FactoryUniqueIdentifier
	GUID GetFactoryUniqueIdentifier() const;
};

/// @brief
/// 	Container of USB analyzers information.
/// @seealso
/// 	IUsbAnalyzerFactory::EnumerateAnalyzers, UsbAnalyzerInformation
typedef std::vector<UsbAnalyzerInformation> vector_usb_analyzer_information;

//--------------------------------------------------------------------
// IUsbAnalyzer
//--------------------------------------------------------------------

/// @brief
/// 	Specifies the USB analyzer errors.
/// @seealso
/// 	usb_analyzer_error_notification_callback
enum usb_analyzer_error
{
	usbAnalyzerErrorNone,								///< No error
	usbAnalyzerErrorBufferOverflow,						///< An overflow occured before all data was read
	usbAnalyzerErrorWronglyConnected,   				///< Analyzer and device connected on the same host controller
	usbAnalyzerErrorPlugAndPlay,        				///< Plug and Play error occured
	usbAnalyzerErrorProhibitedOperation,				///< A prohibited operation was attempted
	usbAnalyzerErrorNotEnoughMemory,					///< The system has not enough memory for continuing operation
	usbAnalyzerErrorSDKNotSupported,					///< The analyzer do not support the USB Analysis SDK
	usbAnalyzerErrorGeneric,            				///< Generic error or warning

	usbAnalyzerErrorCount
};

/// @brief
/// 	Parameter of the USB analyzer error callback.
/// @seealso
/// 	usb_analyzer_error_notification_callback
typedef void* usb_analyzer_error_notification_param;

/// @brief
/// 	Notifies an USB analyzer error asynchronously.
/// @remarks
/// 	This callback notifies that a fatal error occured while recording.
/// 	No more USB elements will be sent by the analyzer to the element sink after this notification.
/// @seealso
/// 	IUsbAnalyzerFactory::BeginAcquisition
typedef void (*usb_analyzer_error_notification_callback)(usb_analyzer_error error, usb_analyzer_error_notification_param param, const void* param2);

/// @brief
/// 	Represents an USB analyzer.
/// @remarks
/// 	An object implementing this interface is returned by 
///		the IUsbAnalyzerFactory::CreateAnalyzer method.
/// @seealso
/// 	IUsbAnalyzerFactory::CreateAnalyzer
class IUsbAnalyzer : public RefCount
{
public:
	/// @brief
	/// 	Starts the acquisition process.
	/// @remarks
	/// 	Begins an asynchronous acquisition of USB elements.
	/// 	The USB element sink will receive in real-time the recorded USB elements.
	/// 	Use the IUsbAnalyzer::EndAcqusition method to stop the acquisition process.
	/// @param 
	///		pElementSink - The sink which will receive USB elements.
	/// @param 
	///		pErrorNotificationCallback - The callback to notify if an asynchronous error occured.
	/// @param 
	///		pErrorNotificationParam - The parameter to pass to the callback.
	/// @exception 
	///		std::exception - Report a runtime error to the developer.
	/// @exception 
	///		std::tstring - Report a logic problem to the user.
	/// @seealso 
	///		IUsbAnalyzer::EndAcquisition, IUsbElementSink
	virtual void BeginAcquisition(IUsbElementSink* pElementSink, usb_analyzer_error_notification_callback pErrorNotificationCallback=NULL, usb_analyzer_error_notification_param pErrorNotificationParam=NULL) /*throw(...)*/ = 0;

	/// @brief
	/// 	Stops the acquisition process.
	/// @remarks
	/// 	End an asynchronous acqusition of USB elements.
	/// @seealso
	/// 	IUsbAnalyzer::BeginAcquisition
	virtual void EndAcquisition() = 0;

	/// @brief
	/// 	Stops the recording of USB elements.
	/// @remarks
	/// 	USB analyzers usually have internal memories to store the recorded 
	///		data before to download it to the analysis computer. Call this method 
	///		to stop recording new analyzed data, but continue to download the internal 
	///		memory of the analyzer. The IUsbAnalyzer::GetPercentMemoryUsed() is used to 
	///		determine if all the internal memory of the analyzer is downloaded.
	/// @seealso
	/// 	IUsbAnalyzer::GetPercentMemoryUsed
	virtual void StopRecording() = 0;

	/// @brief
	/// 	Gets the state of the internal memory of the USB analyzer.
	/// @remarks
	///		Call this method to determine the state of the internal
	///		memory of the USB analyzer.
	///	@return
	///		The percentage of internal memory used.
	///		0 means that the internal memory is empty.
	///		100 means that the internal memory is full.
	/// @seealso
	/// 	IUsbAnalyzer::StopRecording
	virtual BYTE GetPercentMemoryUsed() const = 0;

	/// @brief
	/// 	Compile and send the specified trigger file to the analyzer.
	virtual void ProgramTriggerFromFile(const TCHAR* filename) = 0;

public:
	/// Gets the manufacturer of the analyzer.
	virtual std::tstring GetManufacturer() const = 0;

	/// Gets the model name of the analyzer.
	virtual std::tstring GetModelName() const = 0;

	/// Gets the serial number of the analyzer.
	virtual std::tstring GetSerialNumber() const = 0;
};

//--------------------------------------------------------------------
// IUsbAnalyzerFactory
//--------------------------------------------------------------------

/// @brief
/// 	Factory of an USB analyzer.
/// @sample
/// \code
/// usbdk::IUsbAnalyzer* SelectAndCreateAnalyzer(usbdk::IUsbAnalyzerFactory* pAnalyzerFactory)
/// {
///     usbdk::vector_usb_analyzer_information analyzerInfo;
///     pAnalyzerFactory->EnumerateAnalyzers(analyzerInfo);
/// 
///     if(analyzerInfo.empty())
///     {
///         // No available USB analyzers
///         return NULL;
///     }
/// 
///     if(analyzerInfo.size() > 1)
///     {
///         printf("The first analyzer will be used for the acquisition!\n\n");
///     }
/// 
///     // Return the first available analyzer
///     return pAnalyzerFactory->CreateAnalyzer(analyzerInfo.front().GetUniqueIdentifier());
/// }
/// \endcode
class IUsbAnalyzerFactory : public RefCount
{
public:
	/// @brief
	/// 	Gets the unique identifier of the factory.
	/// @remarks
	/// 	This unique identifier is used to find a factory in a list 
	/// 	of factories when several types of analyzers are found.
	/// @seealso
	/// 	UsbAnalyzerInformation::GetFactoryUniqueIdentifier
	virtual GUID GetFactoryUniqueIdentifier() const = 0;

	/// @brief
	/// 	Enumerates available USB analyzers.
	/// @remarks
	/// 	UsbAnalyzerInformation
	/// @param
	/// 	analyzers - This array will be filled with available USB analyzers.
	/// @seealso
	/// 	UsbAnalyzerInformation, vector_usb_analyzer_information
	virtual void EnumerateAnalyzers(vector_usb_analyzer_information& analyzers) const = 0;

	/// @brief
	/// 	Ask the hardware to provide a visual feedback to help identify the unit.
	/// @remarks
	/// 	Use the IUsbAnalyzerFactory::EnumerateAnalyzers method to obtain available analyzers.
	/// 	Pass the UsbAnalyzerInformation::GetUniqueIdentifier() to get the analyzer object.
	/// @param
	/// 	analyzerUniqueIdentifier - The unique identifier of the analyzer.
	/// @seealso
	/// 	EnumerateAnalyzers, UsbAnalyzerInformation::GetUniqueIdentifier
	virtual void IdentifyAnalyzer(const std::tstring& analyzerUniqueIdentifier) const = 0;

	/// @brief
	/// 	Creates an analyzer from its unique identifier.
	/// @remarks
	/// 	Use the IUsbAnalyzerFactory::EnumerateAnalyzers method to obtain available analyzers.
	/// 	Pass the UsbAnalyzerInformation::GetUniqueIdentifier() to get the analyzer object.
	/// @param
	/// 	analyzerUniqueIdentifier - The unique identifier of the analyzer.
	/// @return
	/// 	A valid IUsbAnalyzer interface if the analyzer is available.
	/// 	NULL otherwise.
	/// @seealso
	/// 	EnumerateAnalyzers, IUsbAnalyzer, UsbAnalyzerInformation::GetUniqueIdentifier
	virtual IUsbAnalyzer* CreateAnalyzer(const std::tstring& analyzerUniqueIdentifier) const = 0;
};

//--------------------------------------------------------------------
// UsbAnalyzerFactoryManager
//--------------------------------------------------------------------

/// @brief
/// 	Manager of USB analyzers factories.
/// @remarks
/// 	Uses this class to simplify the managment of 
///		several USB analyzers factories.
/// @seealso
/// 	IUsbAnalyzerFactory
class UsbAnalyzerFactoryManager
{
private:
	typedef std::list<IUsbAnalyzerFactory*> list_factory;
	list_factory m_factories;

public:
	/// @brief
	/// 	Constructs a UsbAnalyzerFactoryManager object.
	/// @seealso
	/// 	~UsbAnalyzerFactoryManager()
	UsbAnalyzerFactoryManager();

	/// @brief
	/// 	Destroys a UsbAnalyzerFactoryManager object.
	/// @seealso
	/// 	UsbAnalyzerFactoryManager()
	~UsbAnalyzerFactoryManager();

public:
	/// @brief
	/// 	Registers an USB analyzer factory.
	/// @param
	/// 	pAnalyzerFactory - The analyzer factory to register.
	/// @seealso
	/// 	UsbAnalyzerFactoryManager::RemoveFactory
	void RegisterFactory(IUsbAnalyzerFactory* pAnalyzerFactory);

	/// @brief
	/// 	Removes an USB analyzer factory.
	/// @param
	/// 	pAnalyzerFactory - The analyzer factory to remove.
	/// @seealso
	/// 	UsbAnalyzerFactoryManager::RegisterFactory
	void RemoveFactory(IUsbAnalyzerFactory* pAnalyzerFactory);

	/// @brief
	/// 	Removes all USB analyzer factories.
	/// @seealso
	/// 	UsbAnalyzerFactoryManager::RemoveFactory
	void ClearFactories();

	/// @brief
	/// 	Enumerates available USB analyzers from all registered factories.
	/// @param
	/// 	analyzers - This array will be filled with available USB analyzers.
	/// @seealso
	/// 	IUsbAnalyzerFactory::EnumerateAnalyzers
	void EnumerateAnalyzers(vector_usb_analyzer_information& analyzers) const;

	/// @brief
	/// 	Ask the hardware to provide a visual feedback to help identify the unit.
	/// @param
	/// 	factoryUniqueIdentifier - The unique identifier of the factory.
	/// @param
	/// 	analyzerUniqueIdentifier - The unique identifier of the analyzer.
	/// @seealso
	/// 	IUsbAnalyzerFactory::IdentifyAnalyzer
	void IdentifyAnalyzer(GUID factoryUniqueIdentifier, const std::tstring& analyzerUniqueIdentifier) const;

	/// @brief
	/// 	Creates an analyzer from its unique identifier.
	/// @param
	/// 	factoryUniqueIdentifier - The unique identifier of the factory.
	/// @param
	/// 	analyzerUniqueIdentifier - The unique identifier of the analyzer.
	/// @seealso
	/// 	IUsbAnalyzerFactory::CreateAnalyzer
	IUsbAnalyzer* CreateAnalyzer(GUID factoryUniqueIdentifier, const std::tstring& analyzerUniqueIdentifier) const;

	/// @brief
	/// 	Searches a factory from its unique identifier.
	/// @seealso
	/// 	IUsbAnalyzerFactory::GetFactoryUniqueIdentifier
	IUsbAnalyzerFactory* GetFactoryFromUniqueIdentifier(GUID factoryUniqueIdentifier) const;
};

} // End of the usbdk namespace

