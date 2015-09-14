// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbDefs.h
/// @brief
///		USB Analysis SDK standard declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/// @brief
///		Specifies the USB standard device requests.
/// @seealso
///		USB specification chapter 9.4
enum standard_request
{
	requestGetStatus         = 0x00,  ///< 9.4.5
	requestClearFeature      = 0x01,  ///< 9.4.1
	requestSetFeature        = 0x03,  ///< 9.4.9
	// BUGBUG SDK
	requestSetAndTestFeature = 0x04,  ///< LPM : 4.10.3
	requestSetAddress        = 0x05,  ///< 9.4.6
	requestGetDescriptor     = 0x06,  ///< 9.4.3
	requestSetDescriptor     = 0x07,  ///< 9.4.8
	requestGetConfiguration  = 0x08,  ///< 9.4.2
	requestSetConfiguration  = 0x09,  ///< 9.4.7
	requestGetInterface      = 0x0A,  ///< 9.4.4
	requestSetInterface      = 0x0B,  ///< 9.4.10
	requestSynchFrame        = 0x0C,  ///< 9.4.11
};

/// @brief
///		USB setup request format.
/// @seealso
///		USB specification chapter 9.3
#pragma pack(push, 1)
struct setup_request
{
	/// @brief
	///		Characteristics of request.
	/// @seealso
	///		setup_request_direction, setup_request_type, setup_request_recipient
	BYTE bmRequestType;

	/// Specific request.
	BYTE bRequest;

	/// Word-sized field that varies according to request.
	WORD wValue;

	/// Word-sized field that varies according to request; typically used to pass an index or offset.
	WORD wIndex;

	/// Number of bytes to transfer if there is a Data stage.
	WORD wLength;
};
#pragma pack(pop)
C_ASSERT(sizeof(setup_request) == 8);

/// @brief
///		Specifies the setup request data transfer direction.
/// @seealso
///		setup_request.bmRequestType, USB specification chapter 9.3
enum setup_request_direction
{
	directionHostToDevice = 0x00,  ///< Host-to-device
	directionDeviceToHost = 0x80,  ///< Device-to-host
	directionMask         = 0x80,  ///< 9.3
};

/// @brief
///		Specifies the setup request type.
/// @seealso
///		setup_request::bmRequestType, USB specification chapter 9.3
enum setup_request_type
{
	typeStandard = 0x00,  ///< Standard request
	typeClass    = 0x20,  ///< Class request
	typeVendor   = 0x40,  ///< Vendor request
	typeReserved = 0x60,  ///< Reserved
	typeMask     = 0x60,  ///< 9.3
};

/// @brief
///		Specifies the setup request recipient.
/// @seealso
///		setup_request.bmRequestType, USB specification chapter 9.3
enum setup_request_recipient
{
	recipientDevice    = 0x00,  ///< Device
	recipientInterface = 0x01,  ///< Interface
	recipientEndpoint  = 0x02,  ///< Endpoint
	recipientOther     = 0x03,  ///< Other
	recipientMask      = 0x1F,  ///< 9.3
};

/// @brief
///		Specifies the USB transfer types.
/// @seealso
///		endpoint_descriptor.bmAttributes, USB specification chapter 5
enum transfer_type
{
	transferControl     = 0x00,  ///< 5.5
	transferIsochronous = 0x01,  ///< 5.6
	transferBulk        = 0x02,  ///< 5.8
	transferInterrupt   = 0x03,  ///< 5.7
	transferMask        = 0x03,  ///< 9.6.6
};

/// @brief
///		Specifies the standard USB descriptors types.
/// @seealso
///		USB specification chapter 9.6
enum standard_descriptor_type
{
	deviceDescriptorType                     = 0x01,  ///< 9.6.1
	configurationDescriptorType              = 0x02,  ///< 9.6.3
	stringDescriptorType                     = 0x03,  ///< 9.6.7
	interfaceDescriptorType                  = 0x04,  ///< 9.6.5
	endpointDescriptorType                   = 0x05,  ///< 9.6.6
	deviceQualifierDescriptorType            = 0x06,  ///< 9.6.2
	otherSpeedConfigurationDescriptorType    = 0x07,  ///< 9.6.4
	otgDescriptorType                        = 0x09,
	debugDescriptorType                      = 0x0A,
	interfaceAssociationDescriptorType       = 0x0B,
	securityDescriptorType                   = 0x0C,
	keyDescriptorType                        = 0x0D,
	encryptionTypeDescriptorType             = 0x0E,
	binaryObjectStoreDescriptorType          = 0x0F,
	deviceCapabilityDescriptorType           = 0x10,
	wirelessEndpointCompanionDescriptorType  = 0x11,
	lastDescriptorTypeIndex                  = 0x11
};

/// @brief
///		Specifies the standard USB descriptors sizes.
/// @seealso
///		USB specification chapter 9.6
enum standard_descriptor_size
{
	deviceDescriptorSize                      = 18,  ///< 9.6.1
	configurationDescriptorSize               =  9,  ///< 9.6.3
	interfaceDescriptorSize                   =  9,  ///< 9.6.5
	endpointDescriptorSize                    =  7,  ///< 9.6.6
	deviceQualifierDescriptorSize             = 10,  ///< 9.6.2
	otherSpeedConfigurationDescriptorSize     =  9,  ///< 9.6.4
	otgDescriptorSize                         =  3,
	debugDescriptorSize                       =  4,
	interfaceAssociationDescriptorSize        =  8,
	securityDescriptorLength                  =  5,
	keyDescriptorMinLength                    =  6,
	encryptionTypeDescriptorLength            =  5,
	binaryObjectStoreDescriptorLength         =  5,
	deviceCapabilityDescriptorMinLength	      =  3,
	wirelessEndpointCompanionDescriptorLength = 10,
};

/// @brief
///		Standard USB device descriptor.
/// @remarks
///		A device descriptor describes general information 
///		about a USB device. It includes information that applies
///		globally to the device and all of the device’s configurations. 
///		A USB device has only one device descriptor.
/// @seealso
///		USB specification chapter 9.6.1
#pragma pack(push, 1)
struct device_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::deviceDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::deviceDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// @brief
	///		USB Specification Release Number in Binary-Coded Decimal. 
	/// @remarks
	///		This field identifies the release of the USB Specification 
	///		with which the device and its descriptors are compliant.
	///		For example, 2.10 is 210H.
	WORD bcdUSB;

	/// Class code (assigned by the USB-IF).
	BYTE bDeviceClass;

	///	Subclass code (assigned by the USB-IF).
	BYTE bDeviceSubClass;

	/// Subclass code (assigned by the USB-IF).
	BYTE bDeviceProtocol;

	/// Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid).
	BYTE bMaxPacketSize0;

	/// Vendor ID (assigned by the USB-IF)
	WORD idVendor;

	/// Product ID (assigned by the manufacturer).
	WORD idProduct;

	/// Device release number in binary-coded decimal.
	WORD bcdDevice;

	/// Index of string descriptor describing manufacturer.
	BYTE iManufacturer;

	/// Index of string descriptor describing product.
	BYTE iProduct;

	/// Index of string descriptor describing the device’s serial number.
	BYTE iSerialNumber;

	/// Number of possible configurations.
	BYTE bNumConfigurations;
};
#pragma pack(pop)
C_ASSERT(sizeof(device_descriptor) == deviceDescriptorSize);

/// @brief
///		Standard USB configuration descriptor.
/// @remarks
///		The configuration descriptor describes information about 
///		a specific device configuration. The descriptor contains a 
///		bConfigurationValue field with a value that, when used as 
///		a parameter to the SetConfiguration request, causes the 
///		device to assume the described configuration.
/// @seealso
///		USB specification chapter 9.6.3
#pragma pack(push, 1)
struct configuration_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::configurationDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::configurationDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// @brief
	///		Total length of data returned for this configuration. 
	/// @remarks
	///		Includes the combined length
	///		of all descriptors (configuration, interface,
	///		endpoint, and class- or vendor-specific)
	///		returned for this configuration.
	WORD wTotalLength;

	/// Number of interfaces supported by this configuration.
	BYTE bNumInterface;

	///	Value to use as an argument to the SetConfiguration request to select this configuration.
	BYTE bConfigurationValue;

	/// Index of string descriptor describing this configuration.
	BYTE iConfiguration;

	/// Configuration characteristics.
	BYTE bmAttributes;

	/// @brief
	///		Maximum power consumption.
	/// @remarks
	///		The maximum power consumption of the USB device from the bus 
	///		in this specific configuration when the device is fully
	///		operational. Expressed in 2 mA units (i.e., 50 = 100 mA).
	BYTE bMaxPower;
};
#pragma pack(pop)
C_ASSERT(sizeof(configuration_descriptor) == configurationDescriptorSize);

/// @brief
///		Standard USB interface descriptor.
/// @remarks
///		The interface descriptor describes a specific interface 
///		within a configuration. A configuration provides one or more interfaces, 
///		each with zero or more endpoint descriptors describing a unique set of 
///		endpoints within the configuration. When a configuration supports more 
///		than one interface, the endpoint descriptors for a particular interface 
///		follow the interface descriptor in the data returned by the GetConfiguration() request.
///		An interface descriptor is always returned as part of a configuration descriptor. 
///		Interface descriptors cannot be directly accessed with a GetDescriptor() 
///		or SetDescriptor() request.
/// @seealso
///		USB specification chapter 9.6.5
#pragma pack(push, 1)
struct interface_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::interfaceDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::interfaceDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// @brief
	///		Number of this interface. 
	/// @remarks
	///		Zero-based value identifying the index in the array 
	///		of concurrent interfaces supported by this configuration.
	BYTE bInterfaceNumber;

	/// @brief
	///		Value used to select this alternate setting for the 
	///		interface identified in the prior field.
	BYTE bAlternateSetting;

	/// @brief
	///		Number of endpoints used by this interface 
	///		(excluding endpoint zero). 
	///	@remarks
	///		If this value is zero, this interface only 
	///		uses the Default Control Pipe.
	BYTE bNumEndpoints;

	/// Class code (assigned by the USB-IF).
	BYTE bInterfaceClass;

	/// @brief
	///		Subclass code (assigned by the USB-IF).
	///	@remarks
	///		These codes are qualified by the value of the bInterfaceClass field.
	BYTE bInterfaceSubClass;

	/// @brief
	///		Protocol code (assigned by the USB).
	///	@remarks
	///		These codes are qualified by the value of
	///		the bInterfaceClass and the bInterfaceSubClass fields.
	BYTE bInterfaceProtocol;

	/// Index of string descriptor describing this interface.
	BYTE iInterface;
};
#pragma pack(pop)
C_ASSERT(sizeof(interface_descriptor) == interfaceDescriptorSize);

/// @brief
///		Standard USB endpoint descriptor.
/// @remarks
///		Each endpoint used for an interface has its own descriptor. 
///		This descriptor contains the information required by the host 
///		to determine the bandwidth requirements of each endpoint. 
///		An endpoint descriptor is always returned as part of the 
///		configuration information returned by a GetDescriptor(Configuration)
///		request. An endpoint descriptor cannot be directly accessed with a 
///		GetDescriptor() or SetDescriptor() request. There is never an endpoint 
///		descriptor for endpoint zero.
/// @seealso
///		USB specification chapter 9.6.6
#pragma pack(push, 1)
struct endpoint_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::endpointDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::endpointDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// The address of the endpoint on the USB device described by this descriptor.
	BYTE bEndpointAddress;

	///	This field describes the endpoint’s attributes when it is configured using the bConfigurationValue.
	BYTE bmAttributes;

	///	Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected.
	WORD wMaxPacketSize;

	/// @brief
	///		Interval for polling endpoint for data transfers.
	/// @remarks
	///		Expressed in frames or microframes depending on the device 
	///		operating speed (i.e., either 1 millisecond or 125 µs units).
	BYTE bInterval;
};
#pragma pack(pop)
C_ASSERT(sizeof(endpoint_descriptor) == endpointDescriptorSize);

/// @brief
///		Standard USB device qualifier descriptor.
/// @remarks
///		The device_qualifier descriptor describes information about 
///		a high-speed capable device that would change if the device 
///		were operating at the other speed. For example, if the device 
///		is currently operating at full-speed, the device_qualifier 
///		returns information about how it would operate at high-speed and
///		vice-versa.
/// @seealso
///		USB specification chapter 9.6.2
#pragma pack(push, 1)
struct device_qualifier_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::deviceQualifierDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::deviceQualifierDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// USB specification version number (e.g.,	0200H for V2.00)
	WORD bcdUSB;

	/// Class code (assigned by the USB-IF).
	BYTE bDeviceClass;

	/// Subclass code (assigned by the USB-IF).
	BYTE bDeviceSubClass;

	/// Subclass code (assigned by the USB-IF).
	BYTE bDeviceProtocol;

	/// Maximum packet size for other speed.
	BYTE bMaxPacketSize0;

	/// Number of Other-speed Configurations.
	BYTE bNumConfigurations;

	/// Reserved for future use, must be zero.
	BYTE bReserved;
};
#pragma pack(pop)
C_ASSERT(sizeof(device_qualifier_descriptor) == deviceQualifierDescriptorSize);

/// @brief
///		USB OTG descriptor.
/// @remarks
///		During enumeration, an On-The-Go A-device shall request 
///		the OTG configuration descriptor from the B-device.  
///		Any B-device that supports either HNP or SRP must respond 
///		by providing this descriptor in the group of descriptors 
///		returned in response to a GetDescriptor(Configuration) command.
///		When present, the OTG descriptor shall be present in all 
///		configurations.
/// @seealso
///		USB OTG specification chapter 6.4
#pragma pack(push, 1)
struct otg_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::otgDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::otgDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// Attribute Fields
	BYTE bmAttributes;
};
#pragma pack(pop)
C_ASSERT(sizeof(otg_descriptor) == otgDescriptorSize);

/// @brief
///		USB Debug descriptor.
/// @remarks
///		This descriptor is used to describe certain characteristics 
///		of the device that the host debug port driver needs to know 
///		to communicate with the device. Specifically, the debug descriptor 
///		lists the addresses of the endpoints that comprise the Debug Pipe. 
///		The endpoints are identified by endpoint number.
/// @seealso
///		USB Debug Device specification chapter 4.1
#pragma pack(push, 1)
struct debug_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::debugDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::debugDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// @brief
	///		Endpoint number of the Debug Data IN endpoint.
	/// @remarks
	///		This is a Bulk-type endpoint with a maximum packet size of 8 bytes.
	BYTE bDebugInEndpoint;

	/// @brief
	///		Endpoint number of the Debug Data OUT endpoint.
	/// @remarks
	///		This is a Bulk-type endpoint with a maximum packet size of 8 bytes.
	BYTE bDebugOutEndpoint;
};
#pragma pack(pop)
C_ASSERT(sizeof(debug_descriptor) == debugDescriptorSize);

/// @brief
///		USB Interface Association descriptor.
/// @remarks
///		The Interface Association Descriptor is used to describe 
///		that two or more interfaces are associated to the same
///		function. An ‘association’ includes two or more interfaces 
///		and all of their alternate setting interfaces. A device must
///		use an Interface Association descriptor for each device 
///		function that requires more than one interface. An Interface
///		Association descriptor is always returned as part of the 
///		configuration information returned by a GetDescriptor(Configuration) 
///		request. An interface association descriptor cannot be 
///		directly accessed with a GetDescriptor() or SetDescriptor() request. 
///		An interface association descriptor must be located before the set of
///		interface descriptors (including all alternate settings) for the 
///		interfaces it associates. All of the interface numbers in the set of 
///		associated interfaces must be contiguous.
/// @seealso
///		USB Interface Association Descriptor chapter 1
#pragma pack(push, 1)
struct interface_association_descriptor
{
	/// @brief
	///		Size of this descriptor in bytes is 
	///		standard_descriptor_size::interfaceAssociationDescriptorSize.
	/// @seealso
	///		standard_descriptor_size
	BYTE bLength;

	/// @brief
	///		Type of this descriptor is 
	///		standard_descriptor_type::interfaceAssociationDescriptorType.
	/// @seealso
	///		standard_descriptor_type
	BYTE bDescriptorType;

	/// Interface number of the first interface associated with this function.
	BYTE bFirstInterface;

	/// Number of contiguous interfaces associated with this function.
	BYTE bInterfaceCount;

	/// Class code (assigned by USB-IF).
	BYTE bFunctionClass;

	/// Subclass code (assigned by USB-IF).
	BYTE bFunctionSubClass;

	// Protocol code (assigned by USB-IF).
	BYTE bFunctionProtocol;

	/// Index of string descriptor describing this function.
	BYTE iFunction;
};
#pragma pack(pop)
C_ASSERT(sizeof(interface_association_descriptor) == interfaceAssociationDescriptorSize);

#pragma pack(push, 1)
struct security_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	WORD wTotalLength;
	BYTE bNumEncryptionTypes;
};
C_ASSERT(sizeof(security_descriptor) == securityDescriptorLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct key_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	DWORD tTKID : 24;
	DWORD bReserved : 8;
};
C_ASSERT(sizeof(key_descriptor) == keyDescriptorMinLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct encryption_type_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	BYTE bEncryptionType;
	BYTE bEncryptionValue;
	BYTE bAuthKeyIndex;
};
C_ASSERT(sizeof(encryption_type_descriptor) == encryptionTypeDescriptorLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct binary_object_store_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	WORD wTotalLength;
	BYTE bNumDeviceCaps;
};
C_ASSERT(sizeof(binary_object_store_descriptor) == binaryObjectStoreDescriptorLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct device_capability_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	BYTE bDevCapabilityType;
};
C_ASSERT(sizeof(device_capability_descriptor) == deviceCapabilityDescriptorMinLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct wireless_usb_device_capability_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	BYTE bDevCapabilityType;
	BYTE bmAttributes;
	WORD wPHYRates;
	BYTE bmTFITXPowerInfo;
	BYTE bmFFITXPowerInfo;
	WORD bmBandGroup;
	BYTE bReserved;
};
C_ASSERT(sizeof(wireless_usb_device_capability_descriptor) >= deviceCapabilityDescriptorMinLength);
#pragma pack(pop)

#pragma pack(push, 1)
struct wireless_endpoint_companion_descriptor
{
	BYTE bLength;
	BYTE bDescriptorType;
	BYTE bMaxBurst;
	BYTE bMaxSequence;
	WORD wMaxStreamDelay;
	WORD wOverTheAirPacketSize;
	BYTE bOverTheAirInterval;
	BYTE bmCompAttributes;
};
C_ASSERT(sizeof(wireless_endpoint_companion_descriptor) == wirelessEndpointCompanionDescriptorLength);
#pragma pack(pop)

/// @brief
///		Specifies the standard USB feature selectors.
/// @seealso
///		USB specification chapter 9.4 (Table 9-6)
enum standard_feature_selector
{
	selectorEndpointHalt       = 0x00,  ///< Recipient: endpoint
	selectorDeviceRemoteWakeup = 0x01,  ///< Recipient: device 
	selectorTestMode           = 0x02,  ///< Recipient: device
	selectorOtgbHnpEnable      = 0x03,  ///< Recipient: device
	selectorOtgaHnpSupport     = 0x04,  ///< Recipient: device
	selectorOtgaAltHnpSupport  = 0x05,  ///< Recipient: device
	selectorDebugMode          = 0x06   ///< Recipient: device
};

} // End of the usbdk namespace
