// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbPackets.h
/// @brief
///		USB Analysis SDK packets declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/// @brief
///		Specifies the USB packets types.
/// @seealso
///		UsbPacket::GetPacketType, USB specification chapter 8.3.1
enum usb_packet_type
{
	packetStartOfFrame  = 0x00,  ///< USB start of frame packet [8.4.3]
	packetToken         = 0x01,  ///< USB token packet [8.4.1]
	packetData          = 0x02,  ///< USB data packet [8.4.4]
	packetHandshake     = 0x03,  ///< USB handshake packet [8.4.5]
	packetSplit         = 0x04,  ///< USB split transaction special token packet [8.4.2]
	packetExtToken		= 0x05,  ///< USB extension token packet [2.1.1 of 'USB 2.0 Link Power Management Addendum']
	packetCount,
	packetInvalid       = 0xFF   ///< USB packet with invalid PID
};

/// @brief
///		Specifies the USB packets sizes.
/// @seealso
///		UsbPacket::GetRawData, USB specification chapter 8.4
enum usb_packet_size
{
	packetSizeStartOfFrame	= 3,  ///< USB start of frame packet size [8.4.3]
	packetSizeToken			= 3,  ///< USB token packet size [8.4.1]
	packetSizeData			= 3,  ///< USB data packet size [8.4.4]
	packetSizeHandshake		= 1,  ///< USB handshake packet size [8.4.5]
	packetSizeSplit			= 4,  ///< USB split transaction special token packet size [8.4.2]
	packetSizeExtToken		= 3,  ///< USB extension token packet size [2.1.1 of 'USB 2.0 Link Power Management Addendum']
};

/// @brief
///		Specifies the USB packets errors.
/// @seealso
///		UsbPacket::GetErrors
enum usb_packet_error
{
	errorPacketNothing						= 0x000,  ///< No errors
	errorPacketInvalidPID					= 0x001,  ///< The PID field of the packet is invalid
	errorPacketInvalidCRC					= 0x002,  ///< The CRC field of the packet is invalid
	errorPacketInvalidRawDataLength			= 0x004,  ///< The raw data length of the packet is invalid
	errorPacketTokenMissingAddrEndp			= 0x008,  ///< The ADDR and ENDP fields of the token packet are missing
	errorPacketTokenMissingCrc5				= 0x010,  ///< The CRC5 field of the token packet is missing
	errorPacketSplitMissingHubAddrSC		= 0x020,  ///< The HUBADDR and SC fields of the split packet are missing
	errorPacketSplitMissingPortS			= 0x040,  ///< The PORT and S fields of the split packet is missing
	errorPacketSplitMissingEEtCrc5			= 0x080,  ///< The E, ET and CRC5 fields of the split packet are missing
	errorPacketExtTokenMissingPayload		= 0x100,  ///< The payload field of the extended token packet is missing
	errorPacketExtTokenMissingCrc5			= 0x200,  ///< The CRC5 field of the extended token packet is missing
	errorPacketLast							= errorPacketExtTokenMissingCrc5
};

/// @brief
///		Type of USB packet errors.
/// @remarks
///		Each bit represents an error of the usb_packet_error enumeration.
/// @seealso
///		usb_packet_error, UsbPacket::GetErrors
typedef WORD usb_packet_errors;

/// @brief
///		Base class of USB packets.
/// @seealso
///		usb_packet_type, UsbPacket::GetPacketType
class UsbPacket
{
public:
	/// Type of the USB packet raw data container.
	typedef small_vector<BYTE> TContainer;

private:
#pragma pack(push, 1)
	usb_time m_time;
	usb_speed m_speed;
	BYTE m_isExtTokenPacket;
	TContainer m_rawData;
#pragma pack(pop)

public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacket();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacket(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @param 
	///		isExtTokenPacket - If is a extended token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacket(const TContainer& rawData, usb_time time, usb_speed speed, bool isExtTokenPacket = false);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @param 
	///		isExtTokenPacket - If is a extended token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacket(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed, bool isExtTokenPacket = false);

public:
	/// @brief
	///		Clear the content of the USB packet.
	/// @seealso
	///		IsEmpty
	void Clear();

	/// @brief
	///		Determines whether the USB packet is empty.
	/// @return
	///		true if the raw data is empty.
	///		false otherwise.
	/// @seealso
	///		Clear
	inline bool IsEmpty() const;

	/// @brief
	///		Determines whether the USB packet contains an error.
	/// @remarks
	///		If this method returns true, the errors of the 
	///		USB packet can be retrieved with the GetErrors method.
	/// @return
	///		true if the USB packet contains no error.
	///		false otherwise.
	/// @seealso
	///		GetErrors
	inline bool IsValid() const;

	/// @brief
	///		Determines whether the USB packet is an extended packet.
	/// @return
	///		true if its an extended packet.
	///		false otherwise.
	inline bool IsExtTokenPacket() const;

	/// @brief
	///		Returns the PID of the USB packet.
	/// @remarks
	///		pidUnknown is returned if the PID is not a valid PID.
	/// @seealso
	///		usb_pid, GetErrors
	inline usb_pid GetPID() const;

	/// @brief
	///		Returns the SubPID of the USB packet.
	/// @remarks
	///		subpidUnknown is returned if the SubPID is not a valid SubPID.
	/// @seealso
	///		usb_pid, GetErrors
	inline usb_subpid GetSubPID() const;

	/// @brief
	///		Returns the type of the USB packet.
	/// @remarks
	///		The type can be used to cast the USB packet to a specialized USB packet.
	/// @seealso
	///		usb_packet_type
	/// @sample
	/// \code
	/// void ProcessPacket(const usbdk::UsbPacket* pPacket)
	/// {
    ///     switch(pUsbPacket->GetType())
    ///     {
    ///         case usbdk::packetStartOfFrame:
    ///             ProcessPacketStartOfFrame((const usbdk::UsbPacketStartOfFrame*) pUsbPacket);
    ///             break;
    ///
    ///         case usbdk::packetToken:
    ///             ProcessPacketToken((const usbdk::UsbPacketToken*) pUsbPacket);
    ///             break;
    ///
    ///         case usbdk::packetData:
    ///             ProcessPacketData((const usbdk::UsbPacketData*) pUsbPacket);
    ///             break;
    ///
    ///         case usbdk::packetHandshake:
    ///             ProcessPacketHandshake((const usbdk::UsbPacketHandshake*) pUsbPacket);
    ///             break;
    ///
    ///         case usbdk::packetSplit:
    ///             ProcessPacketSplit((const usbdk::UsbPacketSplit*) pUsbPacket);
    ///             break;
    ///
    ///         case usbdk::packetInvalid:
    ///             ProcessPacketInvalid((const usbdk::UsbPacketInvalid*) pUsbPacket);
    ///             break;
    ///
    ///         default:
    ///             // Unknown packet type...
    ///             break;
    ///     }
    /// }
	/// \endcode
	inline usb_packet_type GetPacketType() const;

	/// @brief
	///		Get the raw data of the USB packet.
	/// @remarks
	///		The raw data of a USB packet contains the PID and the packets fields.
	///		Please consult the chapter 8.4 of the USB specification for more information.
	inline TContainer& GetRawData();

	/// @brief
	///		Get the raw data of the USB packet.
	/// @remarks
	///		The raw data of a USB packet contains the PID and the packets fields.
	///		Please consult the chapter 8.4 of the USB specification for more information.
	inline const TContainer& GetRawData() const;

	/// @brief
	///		Get the time of the USB packet.
	/// @seealso
	///		usb_time, SetTime
	inline usb_time GetTime() const;

	/// @brief
	///		Set the time of the USB packet.
	/// @seealso
	///		usb_time, GetTime
	inline void SetTime(usb_time time);

	/// @brief
	///		Get the speed of the USB packet.
	/// @seealso
	///		usb_speed, SetSpeed
	inline usb_speed GetSpeed() const;

	/// @brief
	///		Set the speed of the USB packet.
	/// @seealso
	///		usb_speed, GetSpeed
	inline void SetSpeed(usb_speed speed);

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Returns the USB packet type of a USB packet identifier.
	/// @remarks
	///		The type can be used to cast the USB packet to a specialized USB packet.
	/// @seealso
	///		usb_packet_type
	static inline usb_packet_type GetPacketType(usb_pid pid);

	/// @brief
	///		Get a specialized USB packet from content.
	/// @remarks
	///		This method initialize an USB packet on the stack and return it.
	/// @param
	///		pRawData - The raw data of the USB packet containing the PID and the packets fields.
	/// @param
	///		rawDataSize - The size of the raw data in bytes.
	/// @param
	///		time - The time of the USB packet.
	/// @param
	///		speed - The speed of the USB packet.
	/// @param
	///		isExtPacketToken - Indicates if a EXT packet was received before this packet.
	/// @seealso
	///		CreateSpecializedPacket, USB specification chapter 8.4
	static UsbPacket GetSpecializedPacket(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed, bool isExtTokenPacket = false);

	/// @brief
	///		Create a specialized USB packet from content.
	/// @remarks
	///		This method initialize an USB packet on the heap and return it.
	///		You need to free the memory with the delete operator when the USB packet is no more used.
	/// @param
	///		pRawData - The raw data of the USB packet containing the PID and the packets fields.
	/// @param
	///		rawDataSize - The size of the raw data in bytes.
	/// @param
	///		time - The time of the USB packet.
	/// @param
	///		speed - The speed of the USB packet.
	/// @param
	///		isExtPacketToken - Indicates if a EXT packet was received before this packet.
	/// @seealso
	///		GetSpecializedPacket, USB specification chapter 8.4
	static UsbPacket* CreateSpecializedPacket(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed, bool isExtTokenPacket = false);

	/// @brief
	///		Get a specialized USB packet from content.
	/// @remarks
	///		This method initialize an USB packet on the stack and return it.
	/// @param
	///		rawData - The raw data of the USB packet containing the PID and the packets fields.
	/// @param
	///		time - The time of the USB packet.
	/// @param
	///		speed - The speed of the USB packet.
	/// @param
	///		isExtPacketToken - Indicates if a EXT packet was received before this packet.
	/// @seealso
	///		CreateSpecializedPacket, USB specification chapter 8.4
	template<class Container>
	static UsbPacket GetSpecializedPacket(const Container& rawData, usb_time time, usb_speed speed, bool isExtTokenPacket = false)
	{
		return GetSpecializedPacket(&rawData[0], rawData.size(), time, speed, isExtTokenPacket);
	}

	/// @brief
	///		Create a specialized USB packet from content.
	/// @remarks
	///		This method initialize an USB packet on the heap and return it.
	///		You need to free the memory with the delete operator when the USB packet is no more used.
	/// @param
	///		rawData - The raw data of the USB packet containing the PID and the packets fields.
	/// @param
	///		time - The time of the USB packet.
	/// @param
	///		speed - The speed of the USB packet.
	/// @param
	///		isExtPacketToken - Indicates if a EXT packet was received before this packet.
	/// @seealso
	///		GetSpecializedPacket, USB specification chapter 8.4
	template<class Container>
	static UsbPacket* CreateSpecializedPacket(const Container& rawData, usb_time time, usb_speed speed, bool isExtTokenPacket = false)
	{
		return CreateSpecializedPacket(&rawData[0], rawData.size(), time, speed, isExtTokenPacket);
	}
};

/// @brief
///		USB start-of-Frame packet.
/// @remarks
///		Please consult the chapter 8.4.3 of the USB specification for more information.
class UsbPacketStartOfFrame : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketStartOfFrame();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketStartOfFrame(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketStartOfFrame(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketStartOfFrame(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the frame number of the Start-of-Frame packet.
	/// @remarks
	///		This field is extracted from the raw data. 
	///		The frame number is coded on 11 bits.
	inline usb_frame_number GetFrameNumber() const;

	/// @brief
	///		Returns the CRC-5 of the Start-of-Frame packet.
	/// @remarks
	///		This field is extracted from the raw data. 
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetComputedCRC
	inline usb_crc5 GetCRC() const;

	/// @brief
	///		Returns the computed CRC-5 of the Start-of-Frame packet.
	/// @remarks
	///		This CRC-5 is computed on the raw data.
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetCRC
	inline usb_crc5 GetComputedCRC() const;

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB Start-of-Frame packet.
	/// @remarks
	///		The CRC-5 is automatically computed.
	/// @param 
	///		frameNumber - The FrameNumber field of the USB Start-of-Frame packet.
	/// @return
	///		The raw data of the USB Start-of-Frame packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_frame_number frameNumber);

	/// @brief
	///		Prepares the raw data for the creation of a USB Start-of-Frame packet.
	/// @param 
	///		frameNumber - The FrameNumber field of the USB Start-of-Frame packet.
	/// @param 
	///		crc5 - The CRC5 field of the USB Start-of-Frame packet.
	/// @return
	///		The raw data of the USB Start-of-Frame packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_frame_number frameNumber, usb_crc5 crc5);
};

/// @brief
///		USB token packet.
/// @remarks
///		Please consult the chapter 8.4.1 of the USB specification for more information.
class UsbPacketToken : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketToken();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketToken(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketToken(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketToken(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the device address of the USB token packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The device address is coded on 7 bits.
	inline usb_device_address GetDeviceAddress() const;

	/// @brief
	///		Returns the endpoint number of the USB token packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The endpoint number is coded on 4 bits.
	inline usb_endpoint_number GetEndpointNumber() const;

	/// @brief
	///		Returns the CRC-5 of the USB token packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetComputedCRC
	inline usb_crc5 GetCRC() const;

	/// @brief
	///		Returns the computed CRC-5 of the USB token packet.
	/// @remarks
	///		This CRC-5 is computed on the raw data provided at the creation of the packet.
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetCRC
	inline usb_crc5 GetComputedCRC() const;

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB token packet.
	/// @remarks
	///		The CRC-5 is automatically computed.
	/// @param 
	///		pid - The USB token packet PID.
	/// @param 
	///		deviceAddress - The ADDR field of the USB token packet.
	/// @param 
	///		endpointNumber - The ENDP field of the USB token packet.
	/// @return
	///		The raw data of the USB token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_pid pid, usb_device_address deviceAddress, usb_endpoint_number endpointNumber);

	/// @brief
	///		Prepares the raw data for the creation of a USB token packet.
	/// @param 
	///		pid - The USB token packet PID.
	/// @param 
	///		deviceAddress - The ADDR field of the USB token packet.
	/// @param 
	///		endpointNumber - The ENDP field of the USB token packet.
	/// @param 
	///		crc5 - The CRC5 field of the USB token packet.
	/// @return
	///		The raw data of the USB token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_pid pid, usb_device_address deviceAddress, usb_endpoint_number endpointNumber, usb_crc5 crc5);
};

/// @brief
///		USB extended token packet.
/// @remarks
///		Please consult the chapter 2.1.1 of the USB Link Power Management specification for more information.
class UsbPacketExtToken : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketExtToken();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketExtToken(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketExtToken(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketExtToken(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the payload of the USB extended token packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The token payload is coded on 11 bits.
	WORD GetPayload() const;

	/// @brief
	///		Returns the CRC-5 of the USB extended token packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetComputedCRC
	usb_crc5 GetCRC() const;

	/// @brief
	///		Returns the computed CRC-5 of the USB extended token packet.
	/// @remarks
	///		This CRC-5 is computed on the raw data provided at the creation of the packet.
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetCRC
	usb_crc5 GetComputedCRC() const;

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB extended token packet.
	/// @remarks
	///		The CRC-5 is automatically computed.
	/// @param 
	///		subpid - The USB extended token packet sub-PID.
	/// @param 
	///		payload - The variable field of the USB extended token packet.
	/// @return
	///		The raw data of the USB extended token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_subpid subpid, WORD payload);

	/// @brief
	///		Prepares the raw data for the creation of a USB extended token packet.
	/// @param 
	///		subpid - The USB extended token packet sub-PID.
	/// @param 
	///		deviceAddress - The ADDR field of the USB extended token packet.
	/// @param 
	///		endpointNumber - The ENDP field of the USB extended token packet.
	/// @param 
	///		crc5 - The CRC5 field of the USB extended token packet.
	/// @return
	///		The raw data of the USB extended token packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_subpid subpid, WORD payload, usb_crc5 crc5);
};

/// @brief
///		USB split packet.
/// @remarks
///		Please consult the chapter 8.4.2 of the USB specification for more information.
class UsbPacketSplit : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketSplit();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketSplit(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketSplit(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketSplit(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the hub address of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The hub address is coded on 7 bits.
	inline usb_device_address GetHubAddress() const;

	/// @brief
	///		Returns the SC field of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The SC field is coded on 1 bit.
	inline bool GetStartComplete() const;

	/// @brief
	///		Returns the hub port of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The hub port is coded on 7 bits.
	inline usb_hubport_number GetHubPort() const;

	/// @brief
	///		Returns the S field of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The S field is coded on 1 bit.
	inline bool GetS() const;

	/// @brief
	///		Returns the E field of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The E field is coded on 1 bit.
	inline bool GetE() const;

	/// @brief
	///		Returns the ET field of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The endpoint type is coded on 2 bits.
	inline usb_split_endpoint_type GetEndpointType() const;

	/// @brief
	///		Returns the CRC-5 of the USB split packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetComputedCRC
	inline usb_crc5 GetCRC() const;

	/// @brief
	///		Returns the computed CRC-5 of the USB split packet.
	/// @remarks
	///		This CRC-5 is computed on the raw data provided at the creation of the packet.
	///		The CRC-5 is coded on 5 bits.
	/// @seealso
	///		GetCRC
	inline usb_crc5 GetComputedCRC() const;

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB split packet.
	/// @remarks
	///		The CRC-5 is automatically computed.
	/// @param 
	///		hubAddress - The HubAddr field of the USB split packet.
	/// @param 
	///		startComplete - The SC field of the USB split packet. True for Complete, false for Start.
	/// @param 
	///		hubPort - The Port field of the USB split packet.
	/// @param 
	///		s - The S field of the USB split packet.
	/// @param 
	///		e - The E field USB split packet.
	/// @param 
	///		endpointType - The ET field of the USB split packet.
	/// @return
	///		The raw data of the USB split packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_device_address hubAddress, bool startComplete, usb_hubport_number hubPort, bool s, bool e, usb_split_endpoint_type endpointType);

	/// @brief
	///		Prepares the raw data for the creation of a USB token packet.
	/// @param 
	///		hubAddress - The HubAddr field of the USB split packet.
	/// @param 
	///		startComplete - The SC field of the USB split packet. True for Complete, false for Start.
	/// @param 
	///		hubPort - The Port field of the USB split packet.
	/// @param 
	///		s - The S field of the USB split packet.
	/// @param 
	///		e - The E field USB split packet.
	/// @param 
	///		endpointType - The ET field of the USB split packet.
	/// @param 
	///		crc5 - The CRC5 field of the USB split packet.
	/// @return
	///		The raw data of the USB split packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_device_address hubAddress, bool startComplete, usb_hubport_number hubPort, bool s, bool e, usb_split_endpoint_type endpointType, usb_crc5 crc5);
};

/// @brief
///		USB data packet.
/// @remarks
///		Please consult the chapter 8.4.4 of the USB specification for more information.
class UsbPacketData : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketData();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketData(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketData(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketData(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the useful data of the USB data packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	inline vector_usbdata GetData() const;

	/// @brief
	///		Returns the CRC-16 of the USB data packet.
	/// @remarks
	///		This field is extracted from the raw data provided at the creation of the packet. 
	/// @seealso
	///		UsbPacketData::GetComputedCRC
	inline usb_crc16 GetCRC() const;

	/// @brief
	///		Returns the computed CRC-16 of the USB data packet.
	/// @remarks
	///		This CRC-16 is computed on the raw data provided at the creation of the packet.
	/// @seealso
	///		UsbPacketData::GetCRC
	inline usb_crc16 GetComputedCRC() const;

	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB data packet.
	/// @remarks
	///		The CRC-5 is automatically computed.
	/// @param 
	///		pid - The PID of the data packet.
	/// @param 
	///		pData - The useful data of the data packet.
	/// @param 
	///		dataSize - The size of the useful data buffer.
	/// @return
	///		The raw data of the USB data packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_pid pid, const BYTE* pData, size_t dataSize);

	/// @brief
	///		Prepares the raw data for the creation of a USB data packet.
	/// @param 
	///		pid - The PID of the data packet.
	/// @param 
	///		pData - The useful data of the data packet.
	/// @param 
	///		dataSize - The size of the useful data buffer.
	/// @param 
	///		crc16 - The CRC-16 of the data packet.
	/// @return
	///		The raw data of the USB data packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_pid pid, const BYTE* pData, size_t dataSize, usb_crc16 crc16);
};

/// @brief
///		USB handshake packet.
/// @remarks
///		Please consult the chapter 8.4.5 of the USB specification for more information.
class UsbPacketHandshake : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketHandshake();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketHandshake(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketHandshake(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketHandshake(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;

public:
	/// @brief
	///		Prepares the raw data for the creation of a USB handshake packet.
	/// @param
	///		pid - The USB handskake packet PID.
	/// @return
	///		The raw data of the USB handshake packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket, UsbPacket::GetPacketType
	static TContainer PrepareRawData(usb_pid pid);
};

/// @brief
///		USB invalid packet.
/// @remarks
///		This USB packet is created when the PID of a packet is not valid.
class UsbPacketInvalid : public UsbPacket
{
public:
	/// @brief
	///		Constructs a new empty USB packet.
	UsbPacketInvalid();

	/// @brief
	///		Constructs a copy of an existing USB packet.
	/// @param 
	///		right - An existing USB Packet object to be copied into this object.
	UsbPacketInvalid(const UsbPacket& right);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		rawData - The raw data of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketInvalid(const TContainer& rawData, usb_time time, usb_speed speed);

	/// @brief
	///		Constructs an USB packet from its content.
	/// @remarks
	///		The methods UsbPacket::GetSpecializedPacket or UsbPacket::CreateSpecializedPacket 
	///		are more convenient to create a USB packet.
	/// @param 
	///		pRawData - The raw data of the USB packet.
	/// @param 
	///		rawDataSize - The raw data size of the USB packet.
	/// @param 
	///		time - The time of the USB packet.
	/// @param 
	///		speed - The speed of the USB packet.
	/// @seealso
	///		UsbPacket::GetSpecializedPacket, UsbPacket::CreateSpecializedPacket
	explicit UsbPacketInvalid(const BYTE* pRawData, size_t rawDataSize, usb_time time, usb_speed speed);

public:
	/// @brief
	///		Returns the errors contained in the USB packet.
	/// @seealso
	///		usb_packet_errors, IsValid
	usb_packet_errors GetErrors() const;
};

} // End of the usbdk namespace

#include "UsbPackets.inl"