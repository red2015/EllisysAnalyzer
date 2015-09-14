// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbCrc.h
/// @brief
///		USB Analysis SDK CRC-5 and CRC-16 declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UsbTypes.h"

namespace usbdk {

//////////////////////////////////////////////////////////////////////
// UsbCRC

/// @brief
///		USB CRC-5 and CRC-16 computation helper.
/// @seealso
///		UsbPacketStartOfFrame::GetComputedCRC, UsbPacketToken::GetComputedCRC, 
///		UsbPacketSplit::GetComputedCRC, UsbPacketData::GetComputedCRC
class UsbCRC
{
private:
	// This is a statis class, disallow object creation
	UsbCRC();

public:
	/// @brief
	///      Computes the USB CRC-16 on an array of bytes.
	/// @param
	///      pData - The data buffer on which the CRC-16 will be computed.
	/// @param
	///      dataSize - The size of the data buffer.
	/// @return
	///      The CRC-16 result.
	static usb_crc16 ComputeUsbCRC16(const BYTE* pData, size_t dataSize);

	/// @brief
	///      Computes the USB CRC-16 on a STL compatible container.
	/// @param
	///      first - An iterator on the first element of the container.
	/// @param
	///      last - An iterator on the last element of the container.
	/// @return
	///      The CRC-16 result.
	template<class Iterator>
	static usb_crc16 ComputeUsbCRC16(Iterator first, Iterator last)
	{
		if(first == last)
		{
			return ComputeUsbCRC16Internal(NULL, 0);
		}

		return ComputeUsbCRC16Internal((const BYTE*) &(*first), last-first);
	}

public:
	/// @brief
	///		Computes the USB CRC-5 on an array of bytes.
	/// @param
	///      pData - The data buffer on which the CRC-16 will be computed.
	/// @param
	///      bitCount - The count of bits to extract from pData to compute the CRC-5.
	/// @return
	///		The CRC-5 result.
	static usb_crc5 ComputeUsbCRC5(const BYTE* pData, size_t bitCount);

	/// @brief
	///      Computes the USB CRC-5 on a STL compatible container.
	/// @param
	///      first - An iterator on the first element of the container.
	/// @param
	///      last - An iterator on the last element of the container.
	/// @return
	///      The CRC-5 result.
	template<class Iterator>
	static usb_crc5 ComputeUsbCRC5(Iterator first, Iterator last)
	{
		if(first == last)
		{
			return ComputeUsbCRC5Internal(NULL, 0);
		}

		return ComputeUsbCRC5Internal((const BYTE*) &(*first), (last-first) * 8);
	}
};

} // End of the usbdk namespace
