// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbElementSink.h
/// @brief
///		USB Analysis SDK element sinks declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/// @brief
/// 	Receives recorded USB elements in real-time.
/// @remarks
/// 	IUsbAnalyzer::BeginAcquisition takes an object implementing this interface 
/// 	to start an asynchronous acquisition of USB elements.
/// @seealso
/// 	IUsbAnalyzer::BeginAcquisition, UsbElement
class IUsbElementSink
{
public:
	/// @brief
	/// 	Initializes the USB element sink.
	/// @remarks
	/// 	This method is called by the IUsbAnalyzer::BeginAcquisition method.
	/// @seealso
	/// 	IUsbAnalyzer::BeginAcquisition
	virtual void InitializeElementSink() = 0;

	/// @brief
	/// 	Receives USB elements.
	/// @param
	/// 	pElement - The received USB element.
	/// @seealso
	/// 	UsbElement
	virtual void OnElementArrival(UsbElement* pElement) = 0;

	/// @brief
	/// 	Finalizes the USB element sink.
	/// @remarks
	/// 	This method is called by the IUsbAnalyzer::EndAcquisition method.
	/// @seealso
	/// 	IUsbAnalyzer::EndAcquisition
	virtual void FinalizeElementSink() = 0;
};

/// @brief
/// 	An USB element sink that can be chained with other USB element sinks.
/// @remarks
/// 	An USB element sink can implement several tasks like filters, statistics, etc. 
/// 	When several elements sinks are available, they can be chained with a 
/// 	ChainableUsbElementSinkManager to do a complex task.
/// @seealso
/// 	IUsbElementSink, ChainableUsbElementSinkManager, IUsbAnalyzer::BeginAcquisition, UsbElement
class ChainableUsbElementSink : public IUsbElementSink
{
private:
	IUsbElementSink* m_pNextSink;

public:
	/// @brief
	/// 	Constructs a ChainableUsbElementSink object.
	/// @seealso
	/// 	~ChainableUsbElementSink()
	ChainableUsbElementSink();

	/// @brief
	/// 	Destroys a ChainableUsbElementSink object.
	/// @seealso
	/// 	ChainableUsbElementSink()
	virtual ~ChainableUsbElementSink();

public:
	/// @brief
	/// 	Sets the next sink of the chain.
	/// @remarks
	/// 	This member is used by the ChainableUsbElementSinkManager::AddElementSink.
	/// @param
	/// 	pNextSink - The next sink.
	/// @seealso
	/// 	SendToNextSink, ChainableUsbElementSinkManager::AddElementSink
	inline void SetNextSink(IUsbElementSink* pNextSink);

	/// @brief
	/// 	Gets the next sink of the chain.
	/// @return
	/// 	The next sink if available, otherwise NULL.
	/// @seealso
	/// 	SetNextSink
	inline const IUsbElementSink* GetNextSink() const;

	/// @brief
	/// 	Gets the next sink of the chain.
	/// @return
	/// 	The next sink if available, otherwise NULL.
	/// @seealso
	/// 	SetNextSink
	inline IUsbElementSink* GetNextSink();

protected:
	/// @brief
	/// 	Sends a USB element to the next sink, if available.
	/// @param
	/// 	pElement - The element to send to the next sink.
	/// @seealso
	/// 	IsLastSink, SetNextSink
	inline void SendToNextSink(UsbElement* pElement);

	/// @brief
	/// 	Determines if this sink is the last of the chain.
	/// @seealso
	/// 	SendToNextSink
	inline bool IsLastSink() const;

public:
	virtual void InitializeElementSink() = 0;
	virtual void OnElementArrival(UsbElement* pElement) = 0;
	virtual void FinalizeElementSink() = 0;
};

/// @brief
/// 	Chains several USB element sinks toghether.
/// @seealso
/// 	ChainableUsbElementSink
/// @sample
/// \code
/// usbdk::ChainableUsbElementSinkManager sinkChainer;
/// sinkChainer.AddElementSink(&myStatistics);
/// sinkChainer.AddElementSink(&myFilter);
/// sinkChainer.AddElementSink(&myDisplayer);
/// 
/// usbdk::IUsbAnalyzer pAnalyzer = GetUsbAnalyzer();
/// pAnalyzer->BeginAcquisition(&sinkChainer);
/// \endcode
class ChainableUsbElementSinkManager : public IUsbElementSink
{
private:
	typedef std::list<ChainableUsbElementSink*> vector_sink;
	vector_sink m_sinks;

public:
	/// @brief
	/// 	Constructs a ChainableUsbElementSinkManager object.
	/// @seealso
	/// 	~ChainableUsbElementSinkManager()
	ChainableUsbElementSinkManager();

	/// @brief
	/// 	Destroys a ChainableUsbElementSinkManager object.
	/// @seealso
	/// 	ChainableUsbElementSinkManager()
	virtual ~ChainableUsbElementSinkManager();

public:
	/// @brief
	/// 	Adds an USB element sink to the chain.
	/// @param
	/// 	pElementSink - The element sink to add.
	/// @seealso
	/// 	ChainableUsbElementSink::RemoveElementSink
	void AddElementSink(ChainableUsbElementSink* pElementSink);

	/// @brief
	/// 	Removes an USB element sink from the chain.
	/// @param
	/// 	pElementSink - The element sink to remove.
	/// @seealso
	/// 	ChainableUsbElementSinkManager::AddElementSink
	void RemoveElementSink(ChainableUsbElementSink* pElementSink);

	/// @brief
	/// 	Removes all USB element sinks from the chain.
	/// @seealso
	/// 	ChainableUsbElementSinkManager::RemoveElementSink
	void ClearElementSinks();

public:
	virtual void InitializeElementSink();
	virtual void OnElementArrival(UsbElement* pElement);
	virtual void FinalizeElementSink();
};

/// @brief
/// 	Base class for USB elements processors.
/// @remarks
///		This class helps derived classes to dispatch 
///		generic USB elements to specialized methods.
/// @seealso
/// 	ChainableUsbElementSink
class UsbElementProcessor : public ChainableUsbElementSink
{
public:
	/// @brief
	/// 	Constructs a UsbElementProcessor object.
	/// @seealso
	/// 	~UsbElementProcessor()
	UsbElementProcessor();

	/// @brief
	/// 	Destroys a UsbElementProcessor object.
	/// @seealso
	/// 	UsbElementProcessor()
	virtual ~UsbElementProcessor();

public:
	virtual void InitializeElementSink() = 0;
	virtual void OnElementArrival(UsbElement* pElement);
	virtual void FinalizeElementSink() = 0;

protected:
	/// @brief
	/// 	Processes an USB Invalid Packet element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pInvalidPacket - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessInvalidPacket(UsbInvalidPacket* pInvalidPacket) = 0;

	/// @brief
	/// 	Processes an USB Start Of Frame element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pStartOfFrame - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessStartOfFrame(UsbStartOfFrame* pStartOfFrame) = 0;

	/// @brief
	/// 	Processes an USB Transaction element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pTransaction - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessTransaction(UsbTransaction* pTransaction) = 0;

	/// @brief
	/// 	Processes an USB Split Transaction element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pSplitTransaction - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessSplitTransaction(UsbSplitTransaction* pSplitTransaction) = 0;

	/// @brief
	/// 	Processes an USB LPM Transaction element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pLpmTransaction - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessLpmTransaction(UsbLpmTransaction* pLpmTransaction) = 0;

	/// @brief
	/// 	Processes an USB Suspended element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pSuspended - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessSuspended(UsbSuspended* pSuspended) = 0;

	/// @brief
	/// 	Processes an USB Keep Alive element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pKeepAlive - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessKeepAlive(UsbKeepAlive* pKeepAlive) = 0;

	/// @brief
	/// 	Processes an USB Reset element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pReset - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessReset(UsbReset* pReset) = 0;

	/// @brief
	/// 	Processes an USB Power Change element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pPowerChange - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessPowerChange(UsbPowerChange* pPowerChange) = 0;

	/// @brief
	/// 	Processes an USB High Speed Handshake element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pHighSpeedHandshake - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessHighSpeedHandshake(UsbHighSpeedHandshake* pHighSpeedHandshake) = 0;

	/// @brief
	/// 	Processes an USB Trigger element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pTrigger - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessTrigger(UsbTrigger* pTrigger) = 0;

	/// @brief
	/// 	Processes an unknown USB element.
	/// @remarks
	///		The USB elements received by UsbElementProcessor::OnElementArrival() are dispatched 
	///		to the specialized Process methods for further processing.
	/// @param
	/// 	pElement - The element to process.
	/// @seealso
	/// 	UsbElementProcessor::OnElementArrival
	virtual void ProcessUnknownElement(UsbElement* pElement) = 0;
};

/// @brief
/// 	Stores the USB elements into a container for further analysis.
/// @seealso
/// 	ChainableUsbElementSink
class UsbElementSinkStorage : public ChainableUsbElementSink
{
private:
	bool m_useCircularBuffer;
	size_t m_circularBufferSize;
	container_usb_element* m_pElements;

public:
	/// @brief
	/// 	Constructs a UsbElementSinkStorage object.
	/// @seealso
	/// 	~UsbElementSinkStorage()
	UsbElementSinkStorage();

	/// @brief
	/// 	Destroys a UsbElementSinkStorage object.
	/// @seealso
	/// 	UsbElementSinkStorage()
	virtual ~UsbElementSinkStorage();

public:
	/// @brief
	/// 	Enable or disable the circular buffer mode.
	/// @remarks
	/// 	If enabled, the circular buffer mode will remove the older 
	///		element if a new element is inserted and the element count 
	///		is higher than the circular buffer size defined by SetCircularBufferSize.
	/// @seealso
	/// 	UsbElementSinkStorage::SetCircularBufferSize
	void SetUseCircularBuffer(bool use);

	/// @brief
	/// 	Sets the circular buffer size.
	/// @remarks
	/// 	The circular buffer mode must be enabled by the 
	///		UsbElementSinkStorage::SetUseCircularBuffer separately.
	/// @seealso
	/// 	UsbElementSinkStorage::SetUseCircularBuffer
	void SetCircularBufferSize(size_t s);

	/// @brief
	/// 	Sets the container that will be used to store the elements.
	void SetElementsContainer(container_usb_element* pElements);

public:
	virtual void InitializeElementSink();
	virtual void OnElementArrival(UsbElement* pElement);
	virtual void FinalizeElementSink();
};

} // End of the usbdk namespace

#include "UsbElementSink.inl"