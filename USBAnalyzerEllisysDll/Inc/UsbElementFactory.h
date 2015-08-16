// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbElementFactory.h
/// @brief
///		USB Analysis SDK element factories declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/////////////////////////////////////////////////////////////////////////////
// IUsbElementFactory

/// @brief
/// 	Represents a factory of USB elements.
/// @remarks
/// 	A custom element factory can be defined with the 
/// 	UsbElementFactoryManager::SetCurrentElementFactory method.
/// @seealso
/// 	UsbElementFactoryManager::SetCurrentElementFactory, GetElementFactoryManager
class IUsbElementFactory : public RefCount
{
public:
	/// @brief
	/// 	Creates an uninitialized USB element.
	/// @param 
	/// 	type - The type of the USB element to be created.
	/// @return
	/// 	An uninitialized USB element of the requested type.
	virtual UsbElement* CreateElement(usb_element_type type) = 0;
};

/// @brief
/// 	Factory of standard USB elements.
/// @seealso
/// 	IUsbElementFactory
class UsbElementFactoryStandard : public IUsbElementFactory
{
public:
	virtual UsbElement* CreateElement(usb_element_type type);
};

/// @brief
/// 	Manager of USB elements factories.
/// @remarks
/// 	Uses this class to override the default USB element factory.
/// @seealso
/// 	IUsbElementFactory, GetElementFactoryManager
class UsbElementFactoryManager
{
private:
	RefCountPtr<IUsbElementFactory> m_spCurrentElementFactory;

public:
	/// @brief
	/// 	Constructs a UsbElementFactoryManager object.
	/// @seealso
	/// 	~UsbElementFactoryManager()
	UsbElementFactoryManager();

	/// @brief
	/// 	Destroys a UsbElementFactoryManager object.
	/// @seealso
	/// 	UsbElementFactoryManager()
	~UsbElementFactoryManager();

public:
	/// @brief 
	///		Gets the current USB element factory
	IUsbElementFactory* GetCurrentElementFactory();

	/// @brief 
	///		Sets the USB element factory
	/// @seealso
	///		GetElementFactoryManager
	void SetCurrentElementFactory(IUsbElementFactory* pElementFactory);
};

/// @brief
///		Returns the USB element factory manager used by the development kit.
extern UsbElementFactoryManager* GetElementFactoryManager();

/// @brief
///		Creates an instance of an USB element.
inline UsbElement* CreateElementInstance(usb_element_type t)
{
	return GetElementFactoryManager()->GetCurrentElementFactory()->CreateElement(t);
}

/// @brief
///		Creates an instance of an USB element.
template<class TElement>
TElement* CreateElementInstance()
{
	return (TElement*) CreateElementInstance((usb_element_type) TElement::type);
}

} // End of the usbdk namespace
