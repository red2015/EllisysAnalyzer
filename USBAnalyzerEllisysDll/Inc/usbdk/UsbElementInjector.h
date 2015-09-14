// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbElementInjector.h
/// @brief
///		USB Analysis SDK element injectors declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/////////////////////////////////////////////////////////////////////////////
// IUsbElementInjector

/// @brief
/// 	Parameter of the USB injector progress callback.
/// @seealso
/// 	usb_element_injector_progress_callback
typedef void* usb_element_injector_progress_param;

/// @brief
/// 	Indicates the progress of an injector operation.
/// @seealso
/// 	IUsbElementInjector::Inject
typedef bool (*usb_element_injector_progress_callback)(BYTE percentDone, usb_element_injector_progress_param param);

/// @brief
/// 	Represents an USB element injector.
class IUsbElementInjector
{
public:
	/// @brief
	/// 	Starts the injection process.
	/// @remarks
	/// 	Begins a synchronous injection of USB elements.
	/// 	The USB element sink will receive in the USB elements.
	/// @param 
	///		pElementSink - The sink which will receive USB elements.
	/// @param 
	///		pProgressCallback - The callback to notify the injection progress.
	/// @param 
	///		pProgressParam - The parameter to pass to the callback.
	virtual void Inject(IUsbElementSink* pElementSink, usb_element_injector_progress_callback pProgressCallback, usb_element_injector_progress_param pProgressParam) /*throw(...)*/ = 0;
};

/////////////////////////////////////////////////////////////////////////////
// UsbElementsContainerInjector

/// @brief
/// 	Injector of USB elements container.
class UsbElementsContainerInjector : public IUsbElementInjector
{
private:
	container_usb_element* m_pElements;

public:
	/// @brief
	/// 	Constructs a UsbElementsContainerInjector object.
	/// @seealso
	/// 	~UsbElementsContainerInjector()
	UsbElementsContainerInjector();

	/// @brief
	/// 	Destroys a UsbElementsContainerInjector object.
	/// @seealso
	/// 	UsbElementsContainerInjector()
	virtual ~UsbElementsContainerInjector();

public:
	/// @brief
	/// 	Sets the USB element container.
	void SetElementsContainer(container_usb_element* pElements);

public:
	virtual void Inject(IUsbElementSink* pElementSink, usb_element_injector_progress_callback pProgressCallback, usb_element_injector_progress_param pProgressParam) /*throw(...)*/;
};

} // End of the usbdk namespace
