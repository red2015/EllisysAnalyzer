// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbTypes.h
/// @brief
///		USB Analysis SDK types declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>
#include <list>
#include "custom_vectors.h"

namespace usbdk {

/// @brief
///		Specifies the USB speeds.
/// @remarks
///		Low-speed (1.5 Mbps), Full-speed (12 Mbps) and High-speed (480 Mbps) are link speeds. 
///		The Low-speed prefixed is a special case where Low-speed packets are sent on a Full-speed link with a preamble packet.
/// @seealso
///		UsbPacket::GetSpeed
enum usb_speed_enum
{
	speedLow          = 0x00,  ///< Low-speed (1.5 Mbps)
	speedLowPrefixed  = 0x01,  ///< Low-speed on Full-speed link (1.5 Mbps)
	speedFull         = 0x02,  ///< Full-speed (12 Mbps)
	speedHigh         = 0x03,  ///< High-speed (480 Mbps)
	speedCount,
	speedUnknown      = 0xFF   ///< Unknown speed
};

///	USB speed type.
typedef BYTE usb_speed;

/// @brief
///		Specifies the USB packet identifiers.
/// @remarks
///		These USB packet identifiers are grouped in several types.
///		See UsbPacket.GetPacketType for more information.
/// @seealso
///		UsbPacket::GetPacketType, USB specification chapter 8.3.1
enum usb_pid_enum
{
	pidOUT      = 0xE1,  ///< 1110 0001
	pidIN       = 0x69,  ///< 0110 1001
	pidSOF      = 0xA5,  ///< 1010 0101
	pidSETUP    = 0x2D,  ///< 0010 1101
	pidDATA0    = 0xC3,  ///< 1100 0011
	pidDATA1    = 0x4B,  ///< 0100 1011
	pidACK      = 0xD2,  ///< 1101 0010
	pidNAK      = 0x5A,  ///< 0101 1010
	pidSTALL    = 0x1E,  ///< 0001 1110
	pidDATA2    = 0x87,  ///< 1000 0111
	pidMDATA    = 0x0F,  ///< 0000 1111
	pidNYET     = 0x96,  ///< 1001 0110
	pidPRE_ERR  = 0x3C,  ///< 0011 1100 (ERR reuses PRE value)
	pidSPLIT    = 0x78,  ///< 0111 1000
	pidPING     = 0xB4,  ///< 1011 0100
	pidEXT      = 0xF0,  ///< 1111 0000
	pidUnknown  = 0xFF,  ///< Otherwise
};

///	USB Packet identifier type.
typedef BYTE usb_pid;

/// @brief
///		Specifies the USB packet sub-identifiers.
/// @seealso
///		USB Link Power Management specification chapter 2.1.1
enum usb_subpid_enum
{
	subpidLPM      = 0xC3,  ///< 1100 0011
	subpidUnknown  = 0xFF,  ///< Otherwise
};

///	USB Packet identifier type.
typedef BYTE usb_subpid;

///	USB time type.
typedef double usb_time;

///	Specifies an unknown USB time.
const usb_time unknown_time = 1.7976931348623158e+308;  /* double max value */

/// @brief
///		USB CRC-5 type.
/// @remarks
///		The USB CRC-5 is coded on 5 bits.
typedef BYTE usb_crc5;

/// USB CRC-16 type.
typedef WORD usb_crc16;

/// @brief
///		USB frame number type.
/// @remarks
///		The USB frame number is coded on 11 bits.
typedef WORD usb_frame_number;

/// @brief
///		USB micro-frame number type.
/// @remarks
///		The USB micro-frame number is coded on 3 bits.
typedef BYTE usb_microframe_number;

/// @brief
///		USB device address type.
/// @remarks
///		The USB device address is coded on 7 bits.
typedef BYTE usb_device_address;

/// @brief
///		USB endpoint number type.
/// @remarks
///		The USB endpoint number is coded on 4 bits.
typedef BYTE usb_endpoint_number;

/// @brief
///		USB endpoint address type.
/// @remarks
///		The USB endpoint address is coded on 4 bits with 
///		the most significant bit indicating the direction (1=IN, 0=OUT).
typedef BYTE usb_endpoint_address;

/// @brief
///		USB micro-frame number type.
/// @remarks
///		The USB micro-frame number is coded on 3 bits.
/// USB hub port number type.
typedef BYTE usb_hubport_number;


/// Specifies an unknown USB frame number.
static const usb_frame_number		unknown_frame_number		= (usb_frame_number) -1;

/// Specifies the maximum USB frame number value.
static const usb_frame_number		max_frame_number			= (usb_frame_number) 2047;

/// Specifies the maximum USB frame count.
static const size_t					max_frame_count				= 2048;


/// Specifies an unknown USB micro-frame number.
static const usb_microframe_number	unknown_microframe_number	= (usb_microframe_number) -1;

/// Specifies that no micro-frame number is available.
static const usb_microframe_number	no_microframe_number		= (usb_microframe_number) -2;

/// Specifies that the micro-frame number is invalid.
static const usb_microframe_number	invalid_microframe_number	= (usb_microframe_number) -3;

/// Specifies the maximum USB micro-frame number.
static const usb_microframe_number	max_microframe_number		= (usb_microframe_number) 7;

/// Specifies the maximum USB micro-frame count.
static const size_t					max_microframe_count		= 8;


/// Specifies an unknown USB device address.
static const usb_device_address		unknown_device_address		= (usb_device_address) -1;

/// Specifies the default USB device address.
static const usb_device_address		default_device_address		= (usb_device_address) 0;

/// Specifies the maximum USB device address.
static const usb_device_address		max_device_address			= (usb_device_address) 127;

/// Specifies the maximum USB device count.
static const size_t					max_device_count			= 128;


/// Specifies an unknown USB endpoint number.
static const usb_endpoint_number	unknown_endpoint_number		= (usb_endpoint_number) -1;

/// Specifies the maximum USB endpoint number.
static const usb_endpoint_number	max_endpoint_number			= (usb_endpoint_number) 15;

/// Specifies the maximum USB endpoint count.
static const size_t					max_endpoint_count			= 16;


/// Specifies an unknown USB hub port number.
static const usb_hubport_number		unknown_hubport_number		= (usb_hubport_number) -1;

/// Specifies the maximum USB hub port number.
static const usb_hubport_number		max_hubport_number			= (usb_hubport_number) 127;

/// Specifies the maximum USB hub port count.
static const size_t					max_hubport_count			=  128;


/// @brief
///		Specifies the USB Split transaction types.
/// @seealso
///		UsbSplitTransaction::GetSplitType
enum usb_split_type
{
	splitTypeStart			= 0x00,  ///< Type of the Split transaction is Start
	splitTypeComplete		= 0x01,  ///< Type of the Split transaction is Complete
	splitTypeCount,
	splitTypeUnknown		= 0xFF,  ///< Type of the Split transaction is unknown
};

/// @brief
///		Specifies the USB Split transaction speeds.
/// @seealso
///		UsbSplitTransaction::GetSplitSpeed
enum usb_split_speed
{
	splitSpeedFull			= 0x00,  ///< Speed of the Split transaction is full (12 Mbps)
	splitSpeedLow			= 0x01,  ///< Speed of the Split transaction is low (1.5 Mbps)
	splitSpeedCount,
	splitSpeedUnknown		= 0xFF,  ///< Speed of the Split transaction is unknown
};

/// @brief
///		Specifies the USB Split transaction endpoint type.
/// @seealso
///		UsbSplitTransaction::GetSplitEndpointType
enum usb_split_endpoint_type
{
	splitEndpointTypeControl		= 0x00,  ///< Endpoint Type of the Split transaction is Control
	splitEndpointTypeIsochronous	= 0x01,  ///< Endpoint Type of the Split transaction is Isochronous
	splitEndpointTypeBulk			= 0x02,  ///< Endpoint Type of the Split transaction is Bulk
	splitEndpointTypeInterrupt		= 0x03,  ///< Endpoint Type of the Split transaction is Interrupt
	splitEndpointTypeCount,
	splitEndpointTypeUnknown		= 0xFF,  ///< Endpoint Type of the Split transaction is unknown
};

/// @brief
///		Specifies the USB Split transaction Isochronous Out Payload Continuation.
/// @seealso
///		UsbSplitTransaction::GetIsocOutPayloadContinuation
enum usb_split_isoc_out_payload_continuation
{
	splitIsocOutPayloadContinuationMiddle			= 0x00,  ///< Isochronous Out Payload Continuation of the Split transaction is Middle
	splitIsocOutPayloadContinuationEnd				= 0x01,  ///< Isochronous Out Payload Continuation of the Split transaction is End
	splitIsocOutPayloadContinuationBeginning		= 0x02,  ///< Isochronous Out Payload Continuation of the Split transaction is Beginning
	splitIsocOutPayloadContinuationAll				= 0x03,  ///< Isochronous Out Payload Continuation of the Split transaction is All
	splitIsocOutPayloadContinuationCount,
	splitIsocOutPayloadContinuationNotApplicable	= 0xFE,  ///< Isochronous Out Payload Continuation of the Split transaction is not applicable
	splitIsocOutPayloadContinuationUnknown			= 0xFF,  ///< Isochronous Out Payload Continuation of the Split transaction is unknown
};

/// Helper function to convert an endpoint address to an endpoint number.
inline usb_endpoint_number UsbEndpointAddressToNumber(usb_endpoint_address a)
{
//	ASSERT((a & 0x7F) <= max_endpoint_number);

	if(a == unknown_endpoint_number)
	{
		return unknown_endpoint_number;
	}

	return a & max_endpoint_number;
}

/// Helper function to convert an endpoint number and direction to an endpoint address.
inline usb_endpoint_address UsbEndpointNumberToAddress(usb_endpoint_number n, bool directionIn)
{
//	ASSERT(n <= max_endpoint_number);
	return n | ((directionIn ? 0x80 : 0x00));
}

/// Helper function to determine if the endpoint direction is direction in.
inline bool UsbEndpointIsDirectionIn(usb_endpoint_address a)
{
//	ASSERT((a & 0x7F) <= max_endpoint_number);
	return (a & 0x80) != 0;
}

/// USB data type.
typedef ref_vector<BYTE> vector_usbdata;

} // End of the usbdk namespace
