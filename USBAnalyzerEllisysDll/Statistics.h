// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// Statistics.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UsbAnalysis.h"

static const size_t lowspeed_frame_bytecount  = 188;
static const size_t fullspeed_frame_bytecount = 1500;
static const size_t highspeed_frame_bytecount = 7500;

static const double lowspeed_frame_duration   = 0.001;
static const double fullspeed_frame_duration  = 0.001;
static const double highspeed_frame_duration  = 0.000125;

/////////////////////////////////////////////////////////////////////////////

inline size_t GetPacketByteCount(const usbdk::UsbPacket& packet)
{
	if(packet.IsEmpty())
	{
		return 0;
	}

	size_t count = 1 + packet.GetRawData().size();  // SYNC + raw data

	return count;
}

inline size_t GetTransactionByteCount(const usbdk::UsbTransaction* pTransaction)
{
	size_t count = 0;
	count += GetPacketByteCount(pTransaction->GetTokenPacket());
	count += GetPacketByteCount(pTransaction->GetDataPacket());
	count += GetPacketByteCount(pTransaction->GetHandshakePacket());
	return count;
}

inline size_t GetSplitTransactionByteCount(const usbdk::UsbSplitTransaction* pSplitTransaction)
{
	size_t count = 0;
	count += GetPacketByteCount(pSplitTransaction->GetSplitPacket());
	count += GetPacketByteCount(pSplitTransaction->GetTokenPacket());
	count += GetPacketByteCount(pSplitTransaction->GetDataPacket());
	count += GetPacketByteCount(pSplitTransaction->GetHandshakePacket());
	return count;
}

inline size_t FrameByteCount(usbdk::usb_speed speed)
{
	C_ASSERT(usbdk::speedCount == 4);

	switch(speed)
	{
	case usbdk::speedLow:
	case usbdk::speedLowPrefixed:
		return lowspeed_frame_bytecount;

	case usbdk::speedFull:
		return fullspeed_frame_bytecount;

	default: //case usbdk::speedHigh:
		return highspeed_frame_bytecount;
	}
}

inline usbdk::usb_time FrameDuration(usbdk::usb_speed speed)
{
	C_ASSERT(usbdk::speedCount == 4);

	switch(speed)
	{
	case usbdk::speedLow:
	case usbdk::speedLowPrefixed:
		return lowspeed_frame_duration;

	case usbdk::speedFull:
		return fullspeed_frame_duration;

	default: //case usbdk::speedHigh:
		return highspeed_frame_duration;
	}
}

inline usbdk::usb_time ByteCountToTime(size_t byteCount, size_t frameByteCount, usbdk::usb_time frameDuration)
{
	return (usbdk::usb_time) ((byteCount / ((usbdk::usb_time) frameByteCount)) * frameDuration);
}

inline size_t TimeToByteCount(usbdk::usb_time time, size_t frameByteCount, usbdk::usb_time frameDuration)
{
	return (size_t) ((time * ((usbdk::usb_time) frameByteCount)) / frameDuration);
}

inline usbdk::usb_time ByteCountToTime(size_t byteCount, usbdk::usb_speed speed)
{
	return ByteCountToTime(byteCount, FrameByteCount(speed), FrameDuration(speed));
}

inline size_t TimeToByteCount(usbdk::usb_time time, usbdk::usb_speed speed)
{
	return TimeToByteCount(time, FrameByteCount(speed), FrameDuration(speed));
}
