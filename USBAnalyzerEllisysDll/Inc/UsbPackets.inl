// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "UsbPackets.h"

//////////////////////////////////////////////////////////////////////

namespace usbdk
{
//---------------------------------------------------------------
// UsbPacket
//---------------------------------------------------------------

bool UsbPacket::IsValid() const
{
	return GetErrors() == errorPacketNothing;
}

bool UsbPacket::IsEmpty() const
{
	return m_rawData.empty();
}

bool UsbPacket::IsExtTokenPacket() const
{
	return m_isExtTokenPacket != 0;
}

usb_pid UsbPacket::GetPID() const
{
	if((m_rawData.size() < 1) || IsExtTokenPacket())
	{
		return pidUnknown;
	}

	return (usb_pid) m_rawData[0];
}

usb_subpid UsbPacket::GetSubPID() const
{
	if(m_rawData.size() < 1)
	{
		return pidUnknown;
	}

	if (!IsExtTokenPacket())
	{
		return pidUnknown;
	}

	return (usb_subpid) m_rawData[0];
}

usb_packet_type UsbPacket::GetPacketType() const
{
	if (IsExtTokenPacket())
	{
		return packetExtToken;
	}
	else
	{
		return GetPacketType(GetPID());
	}
}

UsbPacket::TContainer& UsbPacket::GetRawData()
{
	return m_rawData;
}

const UsbPacket::TContainer& UsbPacket::GetRawData() const
{
	return m_rawData;
}

void UsbPacket::SetTime(usb_time time)
{
	m_time = time;
}

void UsbPacket::SetSpeed(usb_speed speed)
{
	m_speed = speed;
}

usb_time UsbPacket::GetTime() const
{
	return m_time;
}

usb_speed UsbPacket::GetSpeed() const
{
	return m_speed;
}

usb_packet_type UsbPacket::GetPacketType(usb_pid pid)
{
	C_ASSERT(packetCount == 6);
	switch(pid)
	{
	case pidSOF:
		return packetStartOfFrame;

	case pidOUT:
	case pidIN:
	case pidSETUP:
	case pidPING:
	case pidEXT:
		return packetToken;

	case pidDATA0:
	case pidDATA1:
	case pidDATA2:
	case pidMDATA:
		return packetData;

	case pidACK:
	case pidNAK:
	case pidSTALL:
	case pidNYET:
	case pidPRE_ERR:
		return packetHandshake;

	case pidSPLIT:
		return packetSplit;
	}

	return packetInvalid;
}

//---------------------------------------------------------------
// UsbPacketStartOfFrame
//---------------------------------------------------------------

usb_frame_number UsbPacketStartOfFrame::GetFrameNumber() const
{
	usb_frame_number data = 0;

	if(GetRawData().size() >= 2)
	{
		((BYTE*) &data)[0] = GetRawData()[1];
	}
	
	if(GetRawData().size() >= 3)
	{
		((BYTE*) &data)[1] = GetRawData()[2];
	}

	return (usb_frame_number) (data & 0x07FF);
}

usb_crc5 UsbPacketStartOfFrame::GetCRC() const
{
	WORD data = 0;

	if(GetRawData().size() >= 2)
	{
		((BYTE*) &data)[0] = GetRawData()[1];
	}
	
	if(GetRawData().size() >= 3)
	{
		((BYTE*) &data)[1] = GetRawData()[2];
	}

	return (usb_crc5) ((data>>11) & 0x1F);
}

usb_crc5 UsbPacketStartOfFrame::GetComputedCRC() const
{
	usb_crc5 computedCrc = 0;

	if(GetRawData().size() >= 3)
	{
		computedCrc = UsbCRC::ComputeUsbCRC5(&GetRawData()[1], 11);;
	}

	return computedCrc;
}

//---------------------------------------------------------------
// UsbPacketToken
//---------------------------------------------------------------

usb_device_address UsbPacketToken::GetDeviceAddress() const
{
	if(GetRawData().size() < 2)
	{
		return unknown_device_address;
	}

	BYTE data = GetRawData()[1];
	return data & 0x7F;
}

usb_endpoint_number UsbPacketToken::GetEndpointNumber() const
{
	if(GetRawData().size() < 3)
	{
		return unknown_endpoint_number;
	}

	WORD data = *((WORD*) &GetRawData()[1]);
	return (usb_endpoint_number) ((data>>7) & 0x0F);
}

usb_crc5 UsbPacketToken::GetCRC() const
{
	usb_crc5 data = 0;

	if(GetRawData().size() >= 3)
	{
		data = GetRawData()[2];
	}

	return (data>>3) & 0x1F;
}

usb_crc5 UsbPacketToken::GetComputedCRC() const
{
	usb_crc5 computedCrc = 0;

	if(GetRawData().size() >= 3)
	{
		computedCrc = UsbCRC::ComputeUsbCRC5(&GetRawData()[1], 11);
	}

	return computedCrc;
}

//---------------------------------------------------------------
// UsbPacketData
//---------------------------------------------------------------

vector_usbdata UsbPacketData::GetData() const
{
	if(GetRawData().size() >= 3)
	{
		return vector_usbdata(GetRawData().begin() + 1, GetRawData().end() - 2);
	}

	return vector_usbdata();
}

usb_crc16 UsbPacketData::GetCRC() const
{
	usb_crc16 crc = 0;

	if(GetRawData().size() >= 3)
	{
		crc = *((WORD *) &GetRawData()[GetRawData().size() - 2]);
	}

	return crc;
}

usb_crc16 UsbPacketData::GetComputedCRC() const
{
	usb_crc16 computedCrc = 0;

	if(GetRawData().size() >= 3)
	{
		computedCrc = UsbCRC::ComputeUsbCRC16(&GetRawData()[1], GetRawData().size() - 3);
	}

	return computedCrc;
}

//---------------------------------------------------------------
// UsbPacketData
//---------------------------------------------------------------

BYTE UsbPacketSplit::GetHubAddress() const
{
	if(GetRawData().size() < 2)
	{
		return unknown_device_address;
	}

	return (GetRawData()[1] & 0x7F);
}

bool UsbPacketSplit::GetStartComplete() const
{
	if(GetRawData().size() < 2)
	{
		return false;
	}

	return (GetRawData()[1] & 0x80) != 0;
}

usb_hubport_number UsbPacketSplit::GetHubPort() const
{
	if(GetRawData().size() < 3)
	{
		return unknown_hubport_number;
	}

	return (GetRawData()[2] & 0x7F);
}

bool UsbPacketSplit::GetS() const
{
	if(GetRawData().size() < 3)
	{
		return false;
	}

	return (GetRawData()[2] & 0x80) != 0;
}

bool UsbPacketSplit::GetE() const
{
	if(GetRawData().size() < 4)
	{
		return false;
	}

	return (GetRawData()[3] & 0x01) != 0;
}

usb_split_endpoint_type UsbPacketSplit::GetEndpointType() const
{
	if(GetRawData().size() < 4)
	{
		return splitEndpointTypeUnknown;
	}

	return (usb_split_endpoint_type) ((GetRawData()[3]>>1) & 0x03);
}

usb_crc5 UsbPacketSplit::GetCRC() const
{
	usb_crc5 data = 0;

	if(GetRawData().size() >= 4)
	{
		data = GetRawData()[3];
	}

	return (data>>3) & 0x1F;
}

usb_crc5 UsbPacketSplit::GetComputedCRC() const
{
	usb_crc5 computedCrc = 0;

	if(GetRawData().size() >= 4)
	{
		computedCrc = UsbCRC::ComputeUsbCRC5(&GetRawData()[1], 19);
	}

	return computedCrc;
}

}
