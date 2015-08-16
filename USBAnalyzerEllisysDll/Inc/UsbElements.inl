// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "UsbAnalysis.h"

//////////////////////////////////////////////////////////////////////

namespace usbdk
{
//---------------------------------------------------------------
// UsbStartOfFrame
//---------------------------------------------------------------

usb_speed UsbStartOfFrame::GetSpeed() const
{
	return m_packet.GetSpeed();
}

bool UsbStartOfFrame::IsValid() const
{
	return (GetErrors() == errorStartOfFrameNothing);
}

bool UsbStartOfFrame::GetNonConsecutive() const
{
	return m_nonConsecutive;
}

void UsbStartOfFrame::SetNonConsecutive(bool nonConsecutive)
{
	m_nonConsecutive = nonConsecutive;
}

usb_frame_number UsbStartOfFrame::GetFrameNumber() const
{
	if(m_packet.IsEmpty())
	{
		return unknown_frame_number;
	}

	return m_packet.GetFrameNumber();
}

usb_microframe_number UsbStartOfFrame::GetMicroFrameNumber() const
{
	return m_microFrameNumber;
}

void UsbStartOfFrame::SetMicroFrameNumber(usb_microframe_number number)
{
	m_microFrameNumber = number;
}

const UsbPacketStartOfFrame& UsbStartOfFrame::GetPacket() const
{
	return m_packet;
}

UsbPacketStartOfFrame& UsbStartOfFrame::GetPacket()
{
	return m_packet;
}

void UsbStartOfFrame::SetPacket(const UsbPacketStartOfFrame& packet)
{
	m_packet = packet;
}

bool UsbStartOfFrame::IsEmpty() const
{
	return m_packet.IsEmpty();
}

//---------------------------------------------------------------
// UsbTransaction
//---------------------------------------------------------------

usb_speed UsbTransaction::GetSpeed() const
{
	if(!m_token.IsEmpty()) return m_token.GetSpeed();
	if(!m_data.IsEmpty()) return m_data.GetSpeed();
	if(!m_handshake.IsEmpty()) return m_handshake.GetSpeed();
	return speedUnknown;
}

const UsbPacketToken& UsbTransaction::GetTokenPacket() const
{
	return m_token;
}

const UsbPacketData& UsbTransaction::GetDataPacket() const
{
	return m_data;
}

const UsbPacketHandshake& UsbTransaction::GetHandshakePacket() const
{
	return m_handshake;
}

UsbPacketToken& UsbTransaction::GetTokenPacket()
{
	return m_token;
}

UsbPacketData& UsbTransaction::GetDataPacket()
{
	return m_data;
}

UsbPacketHandshake& UsbTransaction::GetHandshakePacket()
{
	return m_handshake;
}

void UsbTransaction::SetTokenPacket(const UsbPacketToken& token)
{
	m_token = token;
}

void UsbTransaction::SetDataPacket(const UsbPacketData& data)
{
	m_data = data;
}

void UsbTransaction::SetHandshakePacket(const UsbPacketHandshake& handshake)
{
	m_handshake = handshake;
}

vector_usbdata UsbTransaction::GetData() const
{
	return GetDataPacket().GetData();
}

usb_device_address UsbTransaction::GetDeviceAddress() const
{
	return GetTokenPacket().GetDeviceAddress();
}

usb_endpoint_number UsbTransaction::GetEndpointNumber() const
{
	return GetTokenPacket().GetEndpointNumber();
}

bool UsbTransaction::IsValid() const
{
	return (GetErrors() == errorTransactionNothing);
}

bool UsbTransaction::IsEmpty() const
{
	return m_token.IsEmpty() && m_data.IsEmpty() && m_handshake.IsEmpty();
}

bool UsbTransaction::IsDirectionIn() const
{
	return (GetTokenPacket().GetPID() == pidIN) ? true : false;
}

//---------------------------------------------------------------
// UsbSplitTransaction
//---------------------------------------------------------------

usb_speed UsbSplitTransaction::GetSpeed() const
{
	// Split transactions always have a split packet
	return m_split.GetSpeed();
}

const UsbPacketSplit& UsbSplitTransaction::GetSplitPacket() const
{
	return m_split;
}

const UsbPacketToken& UsbSplitTransaction::GetTokenPacket() const
{
	return m_token;
}

const UsbPacketData& UsbSplitTransaction::GetDataPacket() const
{
	return m_data;
}

const UsbPacketHandshake& UsbSplitTransaction::GetHandshakePacket() const
{
	return m_handshake;
}

UsbPacketSplit& UsbSplitTransaction::GetSplitPacket()
{
	return m_split;
}

UsbPacketToken& UsbSplitTransaction::GetTokenPacket()
{
	return m_token;
}

UsbPacketData& UsbSplitTransaction::GetDataPacket()
{
	return m_data;
}

UsbPacketHandshake& UsbSplitTransaction::GetHandshakePacket()
{
	return m_handshake;
}

void UsbSplitTransaction::SetSplitPacket(const UsbPacketSplit& split)
{
	m_split = split;
}

void UsbSplitTransaction::SetTokenPacket(const UsbPacketToken& token)
{
	m_token = token;
}

void UsbSplitTransaction::SetDataPacket(const UsbPacketData& data)
{
	m_data = data;
}

void UsbSplitTransaction::SetHandshakePacket(const UsbPacketHandshake& handshake)
{
	m_handshake = handshake;
}

vector_usbdata UsbSplitTransaction::GetData() const
{
	return GetDataPacket().GetData();
}

usb_device_address UsbSplitTransaction::GetTokenDeviceAddress() const
{
	return GetTokenPacket().GetDeviceAddress();
}

usb_endpoint_address UsbSplitTransaction::GetTokenEndpointNumber() const
{
	return GetTokenPacket().GetEndpointNumber();
}

usb_device_address UsbSplitTransaction::GetSplitHubAddress() const
{
	return GetSplitPacket().GetHubAddress();
}

usb_hubport_number UsbSplitTransaction::GetSplitHubPort() const
{
	return GetSplitPacket().GetHubPort();
}

usb_split_type UsbSplitTransaction::GetSplitType() const
{
	return GetSplitPacket().GetStartComplete() ? splitTypeComplete : splitTypeStart;
}

usb_split_endpoint_type UsbSplitTransaction::GetSplitEndpointType() const
{
	return (usb_split_endpoint_type) GetSplitPacket().GetEndpointType();
}

usb_split_speed UsbSplitTransaction::GetSplitSpeed() const
{
	if((GetSplitEndpointType() == splitEndpointTypeIsochronous) && (GetTokenPacket().GetPID() == pidOUT))
	{
		return splitSpeedFull;
	}

	return GetSplitPacket().GetS() ? splitSpeedLow : splitSpeedFull;
}

bool UsbSplitTransaction::IsDirectionIn() const
{
	return (GetTokenPacket().GetPID() == pidIN) ? true : false;
}

bool UsbSplitTransaction::IsValid() const
{
	return (GetErrors() == errorSplitTransactionNothing);
}

bool UsbSplitTransaction::IsEmpty() const
{
	return m_split.IsEmpty() && m_token.IsEmpty() && m_data.IsEmpty() && m_handshake.IsEmpty();
}


}