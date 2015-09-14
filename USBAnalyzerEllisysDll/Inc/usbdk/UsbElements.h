// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbElements.h
/// @brief
///		USB Analysis SDK elements declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UsbTypes.h"

namespace usbdk {

/// @brief
/// 	Specifies the USB element types.
/// @seealso
/// 	UsbElement::GetElementType
enum usb_element_type
{
	elementInvalidPacket,          ///< Contains an invalid packet
	elementStartOfFrame,		   ///< Contains a Start-of-Frame packet
	elementTransaction,			   ///< Represents a USB transaction
	elementSplitTransaction,	   ///< Represents a USB split transaction
	elementLpmTransaction,		   ///< Represents a USB LPM transaction
	elementReset,				   ///< Represents a reset  state
	elementSuspended,			   ///< Indicates a suspended state
	elementKeepAlive,			   ///< Represents a low-speed Keep Alive
	elementPowerChange,			   ///< Indicates a power change
	elementHighSpeedHandshake,	   ///< Indicates a high-speed handshake
	elementTrigger,                ///< Indicates a trigger event

	elementCount,
	elementUserDefined = 0x8000	   ///< Base index of user defined elements
};

//---------------------------------------------------------------
// UsbElement
//---------------------------------------------------------------

/// @brief
/// 	Base class of USB elements.
/// @seealso
/// 	usb_element_type, UsbElement::GetElementType
class UsbElement : public RefCount
{
public:
	/// @brief
	/// 	Constructs an UsbElement object.
	/// @seealso
	/// 	~UsbElement()
	UsbElement();

	/// @brief
	/// 	Destroys an UsbElement object.
	/// @seealso
	/// 	UsbElement()
	virtual ~UsbElement();

public:
	/// @brief
	/// 	Gets the type of the USB element.
	/// @remarks
	/// 	Use this method to determine the type of an unknown 
	/// 	USB element and cast it in a specialized USB element.
	/// @sample
	/// \code
	/// void ProcessElement(const usbdk::UsbElement* pElement)
	/// {
    ///     switch(pElement->GetElementType())
    ///     {
    ///     case usbdk::elementInvalidPacket:
    ///         ProcessInvalidPacket((const usbdk::UsbInvalidPacket*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementStartOfFrame:
    ///         ProcessStartOfFrame((const usbdk::UsbStartOfFrame*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementTransaction:
    ///         ProcessTransaction((const usbdk::UsbTransaction*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementSplitTransaction:
    ///         ProcessSplitTransaction((const usbdk::UsbSplitTransaction*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementReset:
    ///         ProcessReset((const usbdk::UsbReset*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementSuspended:
    ///         ProcessSuspended((const usbdk::UsbSuspended*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementKeepAlive:
    ///         ProcessKeepAlive((const usbdk::UsbKeepAlive*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementPowerChange:
    ///         ProcessPowerChange((const usbdk::UsbPowerChange*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementHighSpeedHandshake:
    ///         ProcessHighSpeedHandshake((const usbdk::UsbHighSpeedHandshake*) pElement);
    ///         break;
    /// 
    ///     case usbdk::elementTrigger:
    ///         ProcessTrigger((const usbdk::UsbTrigger*) pElement);
    ///         break;
    /// 
    ///     default:
    ///         // Unknown element...
    ///         break;
    ///     }
    /// }
	/// \endcode
	virtual usb_element_type GetElementType() const = 0;

	/// @brief
	/// 	Gets the absolute time of the USB element.
	/// @seealso
	/// 	usb_time
	virtual usb_time GetTime() const = 0;
};

/// @brief
/// 	Container of USB elements.
/// @seealso
/// 	UsbElement
typedef std::deque<UsbElement*> container_usb_element;

//---------------------------------------------------------------
// UsbInvalidPacket
//---------------------------------------------------------------

/// @brief
/// 	Contains an invalid USB packet.
/// @remarks
/// 	A packet with an invalid PID cannot be grouped in an 
/// 	UsbTransaction element. In this case, the invalid packet 
/// 	will be stored in a UsbInvalidPacket element.
/// @seealso
/// 	UsbElement, UsbPacketInvalid
class UsbInvalidPacket : public UsbElement
{
public: 
	enum { type = elementInvalidPacket };

private:
	UsbPacketInvalid m_packet;

public:
	/// @brief
	/// 	Constructs an UsbInvalidPacket object.
	/// @seealso
	/// 	~UsbInvalidPacket()
	UsbInvalidPacket();

	/// @brief
	/// 	Destroys an UsbInvalidPacket object.
	/// @seealso
	/// 	UsbInvalidPacket()
	virtual ~UsbInvalidPacket();

public:
	virtual usb_element_type GetElementType() const;
	virtual usb_time GetTime() const;

public:
	/// @brief
	/// 	Gets the internal USB packet.
	/// @seealso
	/// 	SetPacket
	const UsbPacketInvalid& GetPacket() const;

	/// @brief
	/// 	Gets the internal USB packet.
	/// @seealso
	/// 	SetPacket
	UsbPacketInvalid& GetPacket();

	/// @brief
	/// 	Sets the internal USB packet.
	/// @seealso
	/// 	GetPacket
	void SetPacket(const UsbPacketInvalid& packet);
};

//---------------------------------------------------------------
// UsbStartOfFrame
//---------------------------------------------------------------

/// @brief
///		Specifies the USB Start-of-Frame errors.
/// @seealso
///		UsbStartOfFrame::GetErrors
enum usb_startofframe_error
{
	errorStartOfFrameNothing			= 0x00,  ///< No errors
	errorStartOfFrameInvalidPacket		= 0x01,  ///< The internal packet is invalid
	errorStartOfFrameInvalidMicroFrame	= 0x02,  ///< The high-speed micro-frame sequence is invalid
	errorStartOfFrameNonConsecutive		= 0x04,  ///< The frame number is not consecutive
	errorStartOfFrameLast				= errorStartOfFrameNonConsecutive
};

/// @brief
///		Type of USB Start-of-Frame errors.
/// @remarks
///		Each bit represents an error of the usb_startofframe_error enumeration.
/// @seealso
///		usb_startofframe_error, UsbStartOfFrame::GetErrors
typedef BYTE usb_startofframe_errors;

/// @brief
/// 	Contains an USB Start-of-Frame packet.
/// @remarks
/// 	The USB Start-of-Frame packets will be stored in
/// 	UsbStartOfFrame elements.
/// @seealso
/// 	UsbElement, UsbPacketStartOfFrame
class UsbStartOfFrame : public UsbElement
{
public: 
	enum { type = elementStartOfFrame };

private:
#pragma pack(push, 1)
	UsbPacketStartOfFrame m_packet;
	usb_microframe_number m_microFrameNumber;
	bool m_nonConsecutive;
#pragma pack(pop)

public:
	/// @brief
	/// 	Constructs an UsbStartOfFrame object.
	/// @seealso
	/// 	~UsbStartOfFrame()
	UsbStartOfFrame();

	/// @brief
	/// 	Destroys an UsbStartOfFrame object.
	/// @seealso
	/// 	UsbStartOfFrame()
	virtual ~UsbStartOfFrame();

public:
	virtual usb_element_type GetElementType() const;
	virtual usb_time GetTime() const;

public:
	/// @brief
	/// 	Clears the data stored in the element.
	/// @seealso
	/// 	IsEmpty
	void Clear();

	/// @brief
	/// 	Determines whether the element is empty.
	/// @return
	/// 	true if the element is empty.
	/// 	false otherwise.
	/// @seealso
	/// 	Clear
	inline bool IsEmpty() const;

	/// @brief
	///		Determines whether the USB Start-of-Frame contains an error.
	/// @remarks
	///		If this method returns true, the errors can be 
	///		retrieved with the GetErrors method.
	/// @return
	///		true if the USB Start-of-Frame contains no error.
	///		false otherwise.
	/// @seealso
	///		GetErrors
	inline bool IsValid() const;

	/// @brief
	///		Returns the errors contained in the USB Start-of-Frame.
	/// @seealso
	///		usb_startofframe_errors, IsValid
	usb_startofframe_errors GetErrors() const;

	/// @brief
	/// 	Gets the frame number of the USB Start-of-Frame packet.
	/// @return
	/// 	The frame number if the USB Start-of-Frame packet is not empty and it is valid.
	/// 	unknown_frame_number otherwise.
	/// @seealso
	/// 	unknown_frame_number, UsbPacketStartOfFrame::GetFrameNumber
	inline usb_frame_number GetFrameNumber() const;

	/// @brief
	/// 	Gets the micro-frame sequence.
	/// @return
	/// 	The micro-frame sequence of the USB Start-of-Frame packet.
	inline usb_microframe_number GetMicroFrameNumber() const;

	/// @brief
	/// 	Sets the micro-frame sequence.
	/// @param
	/// 	number - The micro-frame sequence of the USB Start-of-Frame packet.
	inline void SetMicroFrameNumber(usb_microframe_number number);

	/// @brief
	/// 	Sets the consecutive flag of the Start-of-Frame.
	/// @param
	/// 	nonConsecutive - True if the Start-of-Frame is non-consecutive, false otherwise.
	inline void SetNonConsecutive(bool nonConsecutive);

	/// @brief
	/// 	Gets the consecutive flag of the Start-of-Frame.
	/// @return 
	/// 	True if the Start-of-Frame is non-consecutive.
	///		False otherwise.
	inline bool GetNonConsecutive() const;

	/// @brief
	/// 	Gets the speed of the USB Start-of-Frame.
	/// @seealso
	/// 	usb_speed, UsbPacket::GetSpeed
	inline usb_speed GetSpeed() const;

public:
	/// @brief
	/// 	Gets the internal USB packet.
	/// @seealso
	/// 	SetPacket
	inline const UsbPacketStartOfFrame& GetPacket() const;

	/// @brief
	/// 	Gets the internal USB packet.
	/// @seealso
	/// 	SetPacket
	inline UsbPacketStartOfFrame& GetPacket();

	/// @brief
	/// 	Sets the internal USB packet.
	/// @seealso
	/// 	GetPacket
	inline void SetPacket(const UsbPacketStartOfFrame& packet);
};

//---------------------------------------------------------------
// UsbTransaction
//---------------------------------------------------------------

/// @brief
/// 	Specifies the errors of an USB transaction.
/// @remarks
/// 	Each error represents a bit in the usb_transaction_errors bitset.
/// @seealso
/// 	usb_transaction_errors, UsbTransaction::GetErrors
enum usb_transaction_error
{
	errorTransactionNothing					= 0x00,  ///< No error
	errorTransactionInvalidPacketToken		= 0x01,  ///< The internal token packet is invalid
	errorTransactionInvalidPacketData		= 0x02,  ///< The internal data packet is invalid 
	errorTransactionInvalidPacketHandshake	= 0x04,  ///< The internal handshake packet is invalid
	errorTransactionUpstreamMissing			= 0x08,  ///< The upstream packets are missing
	errorTransactionCorrupted				= 0x10,  ///< The transaction is corrupted
	errorTransactionLast					= errorTransactionCorrupted
};

/// @brief
/// 	Bitset of USB transaction errors.
/// @remarks
/// 	Each bit represents an error of the usb_transaction_error enumeration.
/// @seealso
/// 	usb_transaction_error, UsbTransaction::GetErrors
typedef BYTE usb_transaction_errors;

/// @brief
/// 	Represents a USB transaction.
/// @remarks
/// 	A USB transaction usually contains a token packet, a data packet 
/// 	and a handshake packet. This can vary with the transfer type of the endpoint. 
/// 	For example, isochronous transactions does not contain USB handshake packets.
/// 	Please consult the chapter 8.5 of the USB specification for more information.
/// @seealso
/// 	UsbElement, UsbPacketToken, UsbPacketData, UsbPacketHandshake, 
///		USB specification chapter 8.5
class UsbTransaction : public UsbElement
{
public: 
	enum { type = elementTransaction };

private:
#pragma pack(push, 1)
	UsbPacketToken m_token;
	UsbPacketData m_data;
	UsbPacketHandshake m_handshake;
#pragma pack(pop)

public:
	/// @brief
	/// 	Constructs an UsbTransaction object.
	/// @seealso
	/// 	~UsbTransaction()
	UsbTransaction();

	/// @brief
	/// 	Destroys an UsbTransaction object.
	/// @seealso
	/// 	UsbTransaction()
	virtual ~UsbTransaction();

public:
	virtual usb_element_type GetElementType() const;
	virtual usb_time GetTime() const;

public:
	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	inline const UsbPacketToken& GetTokenPacket() const;

	/// @brief
	/// 	Gets the internal data packet.
	/// @seealso
	/// 	SetDataPacket
	inline const UsbPacketData& GetDataPacket() const;

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	inline const UsbPacketHandshake& GetHandshakePacket() const;

	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	inline UsbPacketToken& GetTokenPacket();

	/// @brief
	/// 	Gets the internal data packet.
	/// @seealso
	/// 	SetDataPacket
	inline UsbPacketData& GetDataPacket();

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	inline UsbPacketHandshake& GetHandshakePacket();

	/// @brief
	/// 	Sets the internal token packet.
	/// @seealso
	/// 	GetTokenPacket
	inline void SetTokenPacket(const UsbPacketToken& token);

	/// @brief
	/// 	Sets the internal data packet.
	/// @seealso
	/// 	GetDataPacket
	inline void SetDataPacket(const UsbPacketData& data);

	/// @brief
	/// 	Sets the internal handshake packet.
	/// @seealso
	/// 	GetHandshakePacket
	inline void SetHandshakePacket(const UsbPacketHandshake& handshake);

public:
	/// @brief
	/// 	Clears the packets stored in the transaction.
	/// @seealso
	/// 	IsEmpty
	void Clear();

	/// @brief
	/// 	Determines whether the transaction is empty.
	/// @return
	/// 	true if the transaction is empty.
	/// 	false otherwise.
	/// @seealso
	/// 	Clear
	inline bool IsEmpty() const;

	/// @brief
	///		Determines whether the transaction contains an error.
	/// @remarks
	///		If this method returns true, the errors of the 
	///		transaction can be retrieved with the GetErrors method.
	/// @return
	///		true if the transaction contains no error.
	///		false otherwise.
	/// @seealso
	///		GetErrors
	inline bool IsValid() const;

	/// @brief
	/// 	Gets the direction of the USB transaction.
	inline bool IsDirectionIn() const;

	/// @brief
	/// 	Gets the speed of the USB transaction.
	/// @seealso
	/// 	usb_speed, UsbPacket::GetSpeed
	inline usb_speed GetSpeed() const;

	/// @brief
	/// 	Gets the useful data of the USB data packet.
	/// @return
	/// 	Returns an empty vector if there is no data packet.
	/// @seealso
	/// 	UsbPacketData::GetData
	inline vector_usbdata GetData() const;

	/// @brief
	/// 	Gets the device address of the USB token packet.
	/// @return
	/// 	The device address if the USB token packet is not empty and it is valid.
	/// 	unknown_device_address otherwise.
	/// @seealso
	/// 	unknown_device_address, UsbPacketToken::GetDeviceAddress
	inline usb_device_address GetDeviceAddress() const;

	/// @brief
	/// 	Gets the endpoint number of the USB token packet.
	/// @return
	/// 	The endpoint number if the USB token packet is not empty and it is valid.
	/// 	unknown_endpoint_number otherwise.
	/// @seealso
	/// 	unknown_endpoint_number, UsbPacketToken::GetEndpointNumber
	inline usb_endpoint_number GetEndpointNumber() const;

	/// @brief
	/// 	Gets the errors of the USB transaction.
	/// @return
	/// 	errorTransactionNothing if the transaction does not contains errors.
	/// 	A bitset of usb_transaction_error otherwise.
	/// @seealso
	/// 	usb_transaction_error
	usb_transaction_errors GetErrors() const;
};

//---------------------------------------------------------------
// UsbSplitTransaction
//---------------------------------------------------------------

/// @brief
/// 	Specifies the errors of an USB split transaction.
/// @remarks
/// 	Each error represents a bit in the usb_split_transaction_errors bitset.
/// @seealso
/// 	usb_split_transaction_errors, UsbSplitTransaction::GetErrors
enum usb_split_transaction_error
{
	errorSplitTransactionNothing                = 0x00,  ///< No error
	errorSplitTransactionInvalidPacketSplit     = 0x01,  ///< The internal split packet is invalid
	errorSplitTransactionInvalidPacketToken     = 0x02,  ///< The internal token packet is invalid
	errorSplitTransactionInvalidPacketData      = 0x04,  ///< The internal data packet is invalid
	errorSplitTransactionInvalidPacketHandshake = 0x08,  ///< The internal handshake packet is invalid
	errorSplitTransactionCorrupted              = 0x10,  ///< The split transaction is corrupted
	errorSplitTransactionBulkIsocInS1			= 0x20,  ///< If ET=(bulk or isocIn) -> S must be 0
	errorSplitTransactionBulkCtrlIntIsocInE1	= 0x40,  ///< If SC=0 and ET=(bulk or control or interrupt or isocIn) -> E must be 0
	errorSplitTransactionLast					= errorSplitTransactionBulkCtrlIntIsocInE1
};

/// @brief
/// 	Bitset of USB split transaction errors.
/// @remarks
/// 	Each bit represents an error of the usb_split_transaction_error enumeration.
/// @seealso
/// 	usb_split_transaction_error, UsbSplitTransaction::GetErrors
typedef BYTE usb_split_transaction_errors;

/// @brief
/// 	Represents a USB split transaction.
/// @remarks
/// 	A USB split transaction usually contains a split packet, a token packet, 
///		a data packet and a handshake packet. This can vary with the transfer type of the endpoint. 
/// @seealso
/// 	UsbElement, UsbPacketSplit, UsbPacketToken, UsbPacketData, UsbPacketHandshake, 
///		USB specification chapter 8.4.2
class UsbSplitTransaction : public UsbElement
{
public: 
	enum { type = elementSplitTransaction };

private:
#pragma pack(push, 1)
	UsbPacketSplit m_split;
	UsbPacketToken m_token;
	UsbPacketData m_data;
	UsbPacketHandshake m_handshake;
#pragma pack(pop)

public:
	/// @brief
	/// 	Constructs an UsbSplitTransaction object.
	/// @seealso
	/// 	~UsbSplitTransaction()
	UsbSplitTransaction();

	/// @brief
	/// 	Destroys an UsbSplitTransaction object.
	/// @seealso
	/// 	UsbSplitTransaction()
	virtual ~UsbSplitTransaction();

public:
	virtual usb_element_type GetElementType() const;
	virtual usb_time GetTime() const;

public:
	/// @brief
	/// 	Gets the internal split packet.
	/// @seealso
	/// 	SetSplitPacket
	inline const UsbPacketSplit& GetSplitPacket() const;

	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	inline const UsbPacketToken& GetTokenPacket() const;

	/// @brief
	/// 	Gets the internal data packet.
	/// @seealso
	/// 	SetDataPacket
	inline const UsbPacketData& GetDataPacket() const;

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	inline const UsbPacketHandshake& GetHandshakePacket() const;

	/// @brief
	/// 	Gets the internal split packet.
	/// @seealso
	/// 	SetSplitPacket
	inline UsbPacketSplit& GetSplitPacket();

	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	inline UsbPacketToken& GetTokenPacket();

	/// @brief
	/// 	Gets the internal data packet.
	/// @seealso
	/// 	SetDataPacket
	inline UsbPacketData& GetDataPacket();

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	inline UsbPacketHandshake& GetHandshakePacket();

	/// @brief
	/// 	Sets the internal split packet.
	/// @seealso
	/// 	GetSplitPacket
	inline void SetSplitPacket(const UsbPacketSplit& split);

	/// @brief
	/// 	Sets the internal token packet.
	/// @seealso
	/// 	GetTokenPacket
	inline void SetTokenPacket(const UsbPacketToken& token);

	/// @brief
	/// 	Sets the internal data packet.
	/// @seealso
	/// 	GetDataPacket
	inline void SetDataPacket(const UsbPacketData& data);

	/// @brief
	/// 	Sets the internal handshake packet.
	/// @seealso
	/// 	GetHandshakePacket
	inline void SetHandshakePacket(const UsbPacketHandshake& handshake);

public:
	/// @brief
	/// 	Clears the packets stored in the transaction.
	/// @seealso
	/// 	IsEmpty
	void Clear();

	/// @brief
	/// 	Determines whether the transaction is empty.
	/// @return
	/// 	true if the transaction is empty.
	/// 	false otherwise.
	/// @seealso
	/// 	Clear
	inline bool IsEmpty() const;

	/// @brief
	///		Determines whether the split transaction contains an error.
	/// @remarks
	///		If this method returns true, the errors of the 
	///		split transaction can be retrieved with the GetErrors method.
	/// @return
	///		true if the split transaction contains no error.
	///		false otherwise.
	/// @seealso
	///		GetErrors
	inline bool IsValid() const;

	/// @brief
	/// 	Gets the direction of the USB split transaction.
	inline bool IsDirectionIn() const;

	/// @brief
	/// 	Gets the speed of the USB split transaction.
	/// @seealso
	/// 	usb_speed, UsbPacket::GetSpeed
	inline usb_speed GetSpeed() const;

	/// @brief
	/// 	Gets the useful data of the USB data packet.
	/// @return
	/// 	Returns an empty vector if there is no data packet.
	/// @seealso
	/// 	UsbPacketData::GetData
	inline vector_usbdata GetData() const;

	/// @brief
	/// 	Gets the errors of the USB split transaction.
	/// @return
	/// 	errorSplitTransactionNothing if the split transaction does not contains errors.
	/// 	A bitset of usb_split_transaction_error otherwise.
	/// @seealso
	/// 	usb_transaction_error
	usb_split_transaction_errors GetErrors() const;

	/// @brief
	/// 	Gets the hub address field of the USB split packet.
	/// @return
	/// 	The hub address if the USB split packet is not empty and it is valid.
	/// 	unknown_device_address otherwise.
	/// @seealso
	/// 	unknown_device_address, UsbPacketSplit::GetHubAddress
	inline usb_device_address GetSplitHubAddress() const;

	/// @brief
	/// 	Gets the hub port field of the USB split packet.
	/// @return
	/// 	The hub port if the USB split packet is not empty and it is valid.
	/// 	unknown_hubport_number otherwise.
	/// @seealso
	/// 	unknown_hubport_number, UsbPacketSplit::GetHubPort
	inline usb_hubport_number GetSplitHubPort() const;

	/// @brief
	/// 	Gets the type field of the USB split packet.
	/// @return
	/// 	The type if the USB split packet is not empty and it is valid.
	/// 	splitTypeUnknown otherwise.
	/// @seealso
	/// 	splitTypeUnknown, UsbPacketSplit::GetStartComplete
	inline usb_split_type GetSplitType() const;

	/// @brief
	/// 	Gets the endpoint type field of the USB split packet.
	/// @return
	/// 	The endpoint type if the USB split packet is not empty and it is valid.
	/// 	splitEndpointTypeUnknown otherwise.
	/// @seealso
	/// 	splitEndpointTypeUnknown, UsbPacketSplit::GetEndpointType
	inline usb_split_endpoint_type GetSplitEndpointType() const;

	/// @brief
	/// 	Gets the speed field of the USB split packet.
	/// @return
	/// 	The speed if the USB split packet is not empty and it is valid.
	/// 	splitSpeedUnknown otherwise.
	/// @seealso
	/// 	splitSpeedUnknown, UsbPacketSplit::GetS
	inline usb_split_speed GetSplitSpeed() const;

	/// @brief
	/// 	Gets the Isochronous Out payload continuation information.
	/// @seealso
	/// 	splitIsocOutPayloadContinuationUnknown
	usb_split_isoc_out_payload_continuation GetIsocOutPayloadContinuation() const;

	/// @brief
	/// 	Gets the device address field of the USB token packet.
	/// @return
	/// 	The device address if the USB token packet is not empty and it is valid.
	/// 	unknown_device_address otherwise.
	/// @seealso
	/// 	unknown_device_address, UsbPacketToken::GetDeviceAddress
	inline usb_device_address GetTokenDeviceAddress() const;

	/// @brief
	/// 	Gets the endpoint number field of the USB token packet.
	/// @return
	/// 	The endpoint number if the USB token packet is not empty and it is valid.
	/// 	unknown_endpoint_number otherwise.
	/// @seealso
	/// 	unknown_endpoint_number, UsbPacketToken::GetEndpointNumber
	inline usb_endpoint_number GetTokenEndpointNumber() const;
};

//---------------------------------------------------------------
// UsbLpmTransaction
//---------------------------------------------------------------

/// @brief
/// 	Specifies the errors of an USB LPM transaction.
/// @remarks
/// 	Each error represents a bit in the usb_lpm_transaction_errors bitset.
/// @seealso
/// 	usb_lpm_transaction_errors, UsbLpmTransaction::GetErrors
enum usb_lpm_transaction_error
{
	errorLpmTransactionNothing					= 0x00,  ///< No error
	errorLpmTransactionInvalidPacketToken		= 0x01,  ///< The internal token packet is invalid
	errorLpmTransactionInvalidPacketExtToken	= 0x02,  ///< The internal extended token packet is invalid
	errorLpmTransactionInvalidPacketHandshake	= 0x04,  ///< The internal handshake packet is invalid
	errorLpmTransactionUpstreamMissing			= 0x08,  ///< The upstream packets are missing
	errorLpmTransactionCorrupted				= 0x10,  ///< The transaction is corrupted
	errorLpmTransactionLast						= errorLpmTransactionCorrupted
};

/// @brief
/// 	Bitset of USB LPM transaction errors.
/// @remarks
/// 	Each bit represents an error of the usb_lpm_transaction_error enumeration.
/// @seealso
/// 	usb_lpm_transaction_error, UsbLpmTransaction::GetErrors
typedef BYTE usb_lpm_transaction_errors;

/// @brief
/// 	Represents a USB LPM transaction.
/// @remarks
/// 	A USB LPM transaction usually contains a token packet, and a handshake packet. 
/// 	Please consult the chapter 2.1.1 of the USB Link Power Management specification for more information.
/// @seealso
/// 	UsbElement, UsbPacketToken, UsbPacketExtToken, UsbPacketHandshake, 
///		USB Link Power Management specification chapter 2.1.1
class UsbLpmTransaction : public UsbElement
{
public: 
	enum { type = elementLpmTransaction };

private:
#pragma pack(push, 1)
	UsbPacketToken m_token;
	UsbPacketExtToken m_extToken;
	UsbPacketHandshake m_handshake;
#pragma pack(pop)

public:
	/// @brief
	/// 	Constructs an UsbLpmTransaction object.
	/// @seealso
	/// 	~UsbLpmTransaction()
	UsbLpmTransaction();

	/// @brief
	/// 	Destroys an UsbLpmTransaction object.
	/// @seealso
	/// 	UsbLpmTransaction()
	virtual ~UsbLpmTransaction();

public:
	virtual usb_element_type GetElementType() const;
	virtual usb_time GetTime() const;

public:
	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	const UsbPacketToken& GetTokenPacket() const;

	/// @brief
	/// 	Gets the internal extended token packet.
	/// @seealso
	/// 	SetExtendedTokenPacket
	const UsbPacketExtToken& GetExtTokenPacket() const;

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	const UsbPacketHandshake& GetHandshakePacket() const;

	/// @brief
	/// 	Gets the internal token packet.
	/// @seealso
	/// 	SetTokenPacket
	UsbPacketToken& GetTokenPacket();

	/// @brief
	/// 	Gets the internal extended token packet.
	/// @seealso
	/// 	SetExtendedTokenPacket
	UsbPacketExtToken& GetExtTokenPacket();

	/// @brief
	/// 	Gets the internal handshake packet.
	/// @seealso
	/// 	SetHandshakePacket
	UsbPacketHandshake& GetHandshakePacket();

	/// @brief
	/// 	Sets the internal token packet.
	/// @seealso
	/// 	GetTokenPacket
	void SetTokenPacket(const UsbPacketToken& token);

	/// @brief
	/// 	Sets the internal extended token packet.
	/// @seealso
	/// 	GetExtendedTokenPacket
	void SetExtTokenPacket(const UsbPacketExtToken& extToken);

	/// @brief
	/// 	Sets the internal handshake packet.
	/// @seealso
	/// 	GetHandshakePacket
	void SetHandshakePacket(const UsbPacketHandshake& handshake);

public:
	/// @brief
	/// 	Clears the packets stored in the transaction.
	/// @seealso
	/// 	IsEmpty
	void Clear();

	/// @brief
	/// 	Determines whether the transaction is empty.
	/// @return
	/// 	true if the transaction is empty.
	/// 	false otherwise.
	/// @seealso
	/// 	Clear
	bool IsEmpty() const;

	/// @brief
	///		Determines whether the transaction contains an error.
	/// @remarks
	///		If this method returns true, the errors of the 
	///		transaction can be retrieved with the GetErrors method.
	/// @return
	///		true if the transaction contains no error.
	///		false otherwise.
	/// @seealso
	///		GetErrors
	bool IsValid() const;

	/// @brief
	/// 	Gets the direction of the USB transaction.
	bool IsDirectionIn() const;

	/// @brief
	/// 	Gets the speed of the USB transaction.
	/// @seealso
	/// 	usb_speed, UsbPacket::GetSpeed
	usb_speed GetSpeed() const;

	/// @brief
	/// 	Gets the attributes of the LPM extended token packet.
	/// @return
	/// 	Returns 0xFFFF if there is no extended token packet.
	WORD GetAttributes() const;

	/// @brief
	/// 	Gets the device address of the USB token packet.
	/// @return
	/// 	The device address if the USB token packet is not empty and it is valid.
	/// 	unknown_device_address otherwise.
	/// @seealso
	/// 	unknown_device_address, UsbPacketToken::GetDeviceAddress
	usb_device_address GetDeviceAddress() const;

	/// @brief
	/// 	Gets the endpoint number of the USB token packet.
	/// @return
	/// 	The endpoint number if the USB token packet is not empty and it is valid.
	/// 	unknown_endpoint_number otherwise.
	/// @seealso
	/// 	unknown_endpoint_number, UsbPacketToken::GetEndpointNumber
	usb_endpoint_number GetEndpointNumber() const;

	/// @brief
	/// 	Gets the errors of the USB LPM transaction.
	/// @return
	/// 	errorLpmTransactionNothing if the transaction does not contains errors.
	/// 	A bitset of usb_lpm_transaction_error otherwise.
	/// @seealso
	/// 	usb_transaction_error
	usb_lpm_transaction_errors GetErrors() const;
};

//---------------------------------------------------------------
// UsbPowerChange
//---------------------------------------------------------------

/// @brief
/// 	Specifies the power change events.
/// @seealso
/// 	usb_power_change
enum usb_power_change_enum
{
	powerChangeOff		= 0x00,  			///< The DUT power supply switched OFF
	powerChangeOn		= 0x01,  			///< The DUT power supply switched ON
	powerChangeCount,
	powerChangeUnknown	= 0xFF				///< Unknown power change
};

/// @brief
/// 	Specifies the power change type.
/// @seealso
/// 	UsbPowerChange::GetPowerChange
typedef BYTE usb_power_change;

/// @brief
/// 	Indicates a power supply change.
/// @remarks
/// 	This event occurs when a cable is plugged on the left Device Under Test 
/// 	connector. This apply power to the Device Under Test.
/// 	The time stamp given by this event may be used to measure power-on and 
/// 	connection events timing like device attachment detection.
/// @seealso
/// 	UsbElement
class UsbPowerChange : public UsbElement
{
public:
	enum { type = elementPowerChange };

private:
	usb_time m_time;
	usb_power_change m_powerChange;

public:
	/// @brief
	/// 	Constructs an UsbPowerChange object.
	/// @seealso
	/// 	~UsbPowerChange()
	UsbPowerChange();

	/// @brief
	/// 	Destroys an UsbPowerChange object.
	/// @seealso
	/// 	UsbPowerChange()
	virtual ~UsbPowerChange();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the element.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);

	/// @brief
	/// 	Gets the power change type.
	/// @seealso
	/// 	SetPowerChange, usb_power_change
	usb_power_change GetPowerChange() const;

	/// @brief
	/// 	Sets the power change type.
	/// @seealso
	/// 	GetPowerChange, usb_power_change
	void SetPowerChange(usb_power_change powerChange);
};

//---------------------------------------------------------------
// UsbReset
//---------------------------------------------------------------

/// @brief
/// 	Indicates a USB reset state.
/// @remarks
/// 	This event occurs when a SE0 state longer than 2.5 microseconds is detected 
/// 	on the bus. Two pull-down resistors on the data lines of hubs, leading a SE0 
/// 	state, ensure they are in a default state when no device is connected. A change 
/// 	of this default state allows the detection of a device connection. When a device 
/// 	is connected, a SE0 state longer than 2.5 microseconds is used to put a device 
/// 	in a known state (Reset).
/// 	The time stamp given by this event may be used to measure power-on and connection 
/// 	events timing like device attachment detection and Resets.
/// @seealso
/// 	UsbElement
class UsbReset : public UsbElement
{
public: 
	enum { type = elementReset };

private:
	usb_time m_time;
	usb_time m_duration;

public:
	/// @brief
	/// 	Constructs an UsbReset object.
	/// @seealso
	/// 	~UsbReset()
	UsbReset();

	/// @brief
	/// 	Destroys an UsbReset object.
	/// @seealso
	/// 	UsbReset()
	virtual ~UsbReset();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the Reset.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);

	/// @brief
	/// 	Gets the duration of the Reset.
	/// @remarks
	/// 	The minimum duration of a Reset is 2.5 microseconds.
	/// @seealso
	/// 	SetDuration
	usb_time GetDuration() const;

	/// @brief
	/// 	Sets the duration of the Reset.
	/// @remarks
	/// 	The minimum duration of a Reset is 2.5 microseconds.
	/// @seealso
	/// 	GetDuration
	void SetDuration(usb_time duration);
};

//---------------------------------------------------------------
// UsbSuspended
//---------------------------------------------------------------

/// @brief
/// 	Indicates a suspended state on the bus.
/// @remarks
/// 	This event begins when more than 3 milliseconds of inactivity are detected 
/// 	on the bus and finishes when a activity is again detected.
/// 	The time stamp given by this event may be used to measure power-on and 
/// 	connection events timing like debounce time. A device should go in a 
/// 	suspended state after this event is detected.
/// @seealso
/// 	UsbElement
class UsbSuspended : public UsbElement
{
public: 
	enum { type = elementSuspended };

private:
	usb_time m_time;
	usb_time m_duration;

public:
	/// @brief
	/// 	Constructs an UsbSuspended object.
	/// @seealso
	/// 	~UsbSuspended()
	UsbSuspended();

	/// @brief
	/// 	Destroys an UsbSuspended object.
	/// @seealso
	/// 	UsbSuspended()
	virtual ~UsbSuspended();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the Suspend.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);

	/// @brief
	/// 	Gets the duration of the suspend state.
	/// @seealso
	/// 	SetDuration
	usb_time GetDuration() const;

	/// @brief
	/// 	Sets the duration of the suspend state.
	/// @seealso
	/// 	GetDuration
	void SetDuration(usb_time duration);
};

//---------------------------------------------------------------
// UsbKeepAlive
//---------------------------------------------------------------

/// @brief
/// 	Indicates a low speed Keep Alive.
/// @remarks
/// 	In the absence of any low-speed traffic, low-speed devices will see at least one 
/// 	keep-alive in every frame in which an SOF occurs, which keeps them from suspending.
/// 	All hub ports to which low-speed devices are connected must generate a low-speed 
/// 	keep-alive strobe, generated at the beginning of the frame, which consists of a valid 
/// 	low-speed EOP. The strobe must be generated at least once in each frame in which an SOF 
/// 	is received. This strobe is used to prevent low-speed devices from suspending if there 
/// 	is no other low-speed traffic on the bus.
/// @seealso
/// 	UsbElement
class UsbKeepAlive : public UsbElement
{
public: 
	enum { type = elementKeepAlive };

private:
	usb_time m_time;

public:
	/// @brief
	/// 	Constructs an UsbKeepAlive object.
	/// @seealso
	/// 	~UsbKeepAlive()
	UsbKeepAlive();

	/// @brief
	/// 	Destroys an UsbKeepAlive object.
	/// @seealso
	/// 	UsbKeepAlive()
	virtual ~UsbKeepAlive();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the element.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);
};

//---------------------------------------------------------------
// UsbHighSpeedHandshake
//---------------------------------------------------------------

/// @brief
/// 	Specifies the high speed handshake status.
/// @seealso
/// 	usb_highspeed_handshake_status
enum usb_highspeed_handshake_status_enum
{
	hsHandshakeSuccess						= 0x00,  ///< The handshake completed successfully
	hsHandshakeUpstreamPortChirpKTimeout	= 0x01,  ///< The upstream port (device) did not generate Chirp K state.
	hsHandshakeDownstreamPortChirpKTimeout	= 0x02,  ///< The upstream port (device) has properly generated Chirp K state, but the downstream port (host) did not answer.
	hsHandshakeNotEnoughDownstreamChirps	= 0x03,  ///< The high speed handshake started but the downstream port did not generate enough Chirps to successfully complete the handshake.
	hsHandshakeCount,
	hsHandshakeUnknown						= 0xFF  ///< Unknown status
};

/// @brief
/// 	Specifies the high speed handshake status type.
/// @seealso
/// 	UsbHighSpeedHandshake::GetStatus
typedef BYTE usb_highspeed_handshake_status;

/// @brief
/// 	Indicates a High Speed Detection Handshake.
/// @seealso
/// 	UsbElement
class UsbHighSpeedHandshake : public UsbElement
{
public:
	enum { type = elementHighSpeedHandshake };

private:
	usb_time m_time;
	usb_highspeed_handshake_status m_status;

public:
	/// @brief
	/// 	Constructs an UsbHighSpeedHandshake object.
	/// @seealso
	/// 	~UsbHighSpeedHandshake()
	UsbHighSpeedHandshake();

	/// @brief
	/// 	Destroys an UsbHighSpeedHandshake object.
	/// @seealso
	/// 	UsbHighSpeedHandshake()
	virtual ~UsbHighSpeedHandshake();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the element.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);

	/// @brief
	/// 	Gets the status of the high speed handshake.
	/// @seealso
	/// 	SetStatus, usb_highspeed_handshake_status
	usb_highspeed_handshake_status GetStatus() const;

	/// @brief
	/// 	Sets the status of the high speed handshake.
	/// @seealso
	/// 	GetStatus, usb_highspeed_handshake_status
	void SetStatus(usb_highspeed_handshake_status status);
};

//---------------------------------------------------------------
// UsbTrigger
//---------------------------------------------------------------

/// @brief
/// 	Indicates a hardware Trigger event.
/// @seealso
/// 	UsbElement
class UsbTrigger : public UsbElement
{
public:
	enum { type = elementTrigger };

private:
	usb_time m_time;

public:
	/// @brief
	/// 	Constructs an UsbTrigger object.
	/// @seealso
	/// 	~UsbTrigger()
	UsbTrigger();

	/// @brief
	/// 	Destroys an UsbTrigger object.
	/// @seealso
	/// 	UsbTrigger()
	virtual ~UsbTrigger();

public:
	virtual usb_time GetTime() const;
	virtual usb_element_type GetElementType() const;

public:
	/// @brief
	/// 	Sets the absolute time of the element.
	/// @seealso
	/// 	GetTime, usb_time
	void SetTime(usb_time time);
};

//---------------------------------------------------------------
// UsbElementContainer
//---------------------------------------------------------------

class UsbElementContainer : public UsbElement  
{
private:
	container_usb_element m_elements;

public:
	UsbElementContainer();
	virtual ~UsbElementContainer();

public:
	virtual usb_element_type GetElementType() const = 0;
	virtual usb_time GetTime() const;

public:
	virtual void Clear();
	virtual bool IsEmpty() const;
	virtual void Add(UsbElement* pElement);
	virtual void AddElements(container_usb_element& elements);
	virtual container_usb_element& GetElements();
	virtual const container_usb_element& GetElements() const;
};

} // End of the usbdk namespace

#include "UsbElements.inl"
