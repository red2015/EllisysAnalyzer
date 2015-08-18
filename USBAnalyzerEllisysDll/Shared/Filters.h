// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// Filters.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UsbAnalysis.h"

/////////////////////////////////////////////////////////////////////////////

enum filter_mode
{
	filterAll,
	filterMatches,
	filterMismatches,
	filterNothing,
};

enum criteria_match_mode
{
	matchInclude,
	matchExclude,
};

class UsbElementFilter : public usbdk::ChainableUsbElementSink
{
private:
	typedef std::set<usbdk::usb_element_type> set_element_type;

private:
	filter_mode m_filterMode;

private:
	criteria_match_mode m_typeMatchMode;
	set_element_type m_typeCriteria;

public:
	UsbElementFilter() :
		m_filterMode(filterNothing),
		m_typeMatchMode(matchInclude),
		m_typeCriteria()
	{
	}

	~UsbElementFilter()
	{
	}

public:
	void SetFilterMode(filter_mode mode)
	{
		m_filterMode = mode;
	}

public:
	void SetElementTypeMatchMode(criteria_match_mode mode)
	{
		m_typeMatchMode = mode;
	}

	void AddElementTypeCriteria(usbdk::usb_element_type type)
	{
		m_typeCriteria.insert(type);
	}

	void ClearCriteria()
	{
		m_typeCriteria.clear();
	}

public:
	virtual void InitializeElementSink()
	{
	}

	virtual void OnElementArrival(usbdk::UsbElement* pElement)
	{
		bool match = true;
		bool matchUpdated = false;

		if(!m_typeCriteria.empty())
		{
			bool found = (m_typeCriteria.find(pElement->GetElementType()) != m_typeCriteria.end());
			match &= (found == (m_typeMatchMode==matchInclude));
			matchUpdated = true;
		}

		bool filtered = false;

		if(matchUpdated)
		{
			switch(m_filterMode)
			{
			case filterAll: filtered = true; break;
			case filterMatches: filtered = match; break;
			case filterMismatches: filtered = !match; break;
			case filterNothing: filtered = false; break;
			}
		}

		if(!filtered)
		{
			SendToNextSink(pElement);
		}
	}

	virtual void FinalizeElementSink()
	{
	}
};

class UsbTransactionFilter : public usbdk::ChainableUsbElementSink
{
private:
	typedef std::set<usbdk::usb_pid> set_pid;
	typedef std::set<usbdk::usb_speed> set_speed;
	typedef std::set<BYTE> set_byte;

private:
	filter_mode m_filterMode;

private:
	criteria_match_mode m_tokenPidMatchMode;
	criteria_match_mode m_dataPidMatchMode;
	criteria_match_mode m_handshakePidMatchMode;
	criteria_match_mode m_speedMatchMode;
	criteria_match_mode m_deviceAddressMatchMode;
	criteria_match_mode m_endpointNumberMatchMode;

private:
	set_pid m_tokenPidCriteria;
	set_pid m_dataPidCriteria;
	set_pid m_handshakePidCriteria;
	set_speed m_speedCriteria;
	set_byte m_deviceAddressCriteria;
	set_byte m_endpointNumberCriteria;

public:
	UsbTransactionFilter() :
		m_filterMode(filterNothing),
		m_tokenPidMatchMode(matchInclude),
		m_dataPidMatchMode(matchInclude),
		m_handshakePidMatchMode(matchInclude),
		m_speedMatchMode(matchInclude),
		m_deviceAddressMatchMode(matchInclude),
		m_endpointNumberMatchMode(matchInclude),
		m_tokenPidCriteria(),
		m_dataPidCriteria(),
		m_handshakePidCriteria(),
		m_speedCriteria(),
		m_deviceAddressCriteria(),
		m_endpointNumberCriteria()
	{
	}

	~UsbTransactionFilter()
	{
	}

public:
	void SetFilterMode(filter_mode mode)
	{
		m_filterMode = mode;
	}

public:
	void SetTokenPidMatchMode(criteria_match_mode mode)
	{
		m_tokenPidMatchMode = mode;
	}

	void SetDataPidMatchMode(criteria_match_mode mode)
	{
		m_dataPidMatchMode = mode;
	}

	void SetHandshakePidMatchMode(criteria_match_mode mode)
	{
		m_handshakePidMatchMode = mode;
	}

	void SetSpeedMatchMode(criteria_match_mode mode)
	{
		m_speedMatchMode = mode;
	}

	void SetDeviceAddressMatchMode(criteria_match_mode mode)
	{
		m_deviceAddressMatchMode = mode;
	}

	void SetEndpointNumberMatchMode(criteria_match_mode mode)
	{
		m_endpointNumberMatchMode = mode;
	}

public:
	void AddTokenPidCriteria(usbdk::usb_pid pid)
	{
		m_tokenPidCriteria.insert(pid);
	}

	void AddDataPidCriteria(usbdk::usb_pid pid)
	{
		m_dataPidCriteria.insert(pid);
	}

	void AddHandshakePidCriteria(usbdk::usb_pid pid)
	{
		m_handshakePidCriteria.insert(pid);
	}

	void AddSpeedCriteria(usbdk::usb_speed speed)
	{
		m_speedCriteria.insert(speed);
	}

	void AddDeviceAddressCriteria(BYTE deviceAddress)
	{
		m_deviceAddressCriteria.insert(deviceAddress);
	}

	void AddEndpointNumberCriteria(BYTE endpointNumber)
	{
		m_endpointNumberCriteria.insert(endpointNumber);
	}

	void ClearCriteria()
	{
		m_tokenPidCriteria.clear();
		m_dataPidCriteria.clear();
		m_handshakePidCriteria.clear();
		m_speedCriteria.clear();
		m_deviceAddressCriteria.clear();
		m_endpointNumberCriteria.clear();
	}

public:
	virtual void InitializeElementSink()
	{
	}

	virtual void OnElementArrival(usbdk::UsbElement* pElement)
	{
		if(pElement->GetElementType() != usbdk::elementTransaction)
		{
			SendToNextSink(pElement);
			return;
		}

		usbdk::UsbTransaction* pTransaction = (usbdk::UsbTransaction*) pElement;

		bool match = true;
		bool matchUpdated = false;

		if(!m_tokenPidCriteria.empty() && !pTransaction->GetTokenPacket().IsEmpty())
		{
			bool found = (m_tokenPidCriteria.find(pTransaction->GetTokenPacket().GetPID()) != m_tokenPidCriteria.end());
			match &= (found == (m_tokenPidMatchMode==matchInclude));
			matchUpdated = true;
		}

		if(!m_dataPidCriteria.empty() && !pTransaction->GetDataPacket().IsEmpty())
		{
			bool found = (m_dataPidCriteria.find(pTransaction->GetDataPacket().GetPID()) != m_dataPidCriteria.end());
			match &= (found == (m_dataPidMatchMode==matchInclude));
			matchUpdated = true;
		}

		if(!m_handshakePidCriteria.empty() && !pTransaction->GetHandshakePacket().IsEmpty())
		{
			bool found = (m_handshakePidCriteria.find(pTransaction->GetHandshakePacket().GetPID()) != m_handshakePidCriteria.end());
			match &= (found == (m_handshakePidMatchMode==matchInclude));
			matchUpdated = true;
		}

		if(!m_speedCriteria.empty())
		{
			bool found = (m_speedCriteria.find(pTransaction->GetSpeed()) != m_speedCriteria.end());
			match &= (found == (m_speedMatchMode==matchInclude));
			matchUpdated = true;
		}

		if(!m_deviceAddressCriteria.empty())
		{
			bool found = (m_deviceAddressCriteria.find(pTransaction->GetDeviceAddress()) != m_deviceAddressCriteria.end());
			match &= (found == (m_deviceAddressMatchMode==matchInclude));
			matchUpdated = true;
		}

		if(!m_endpointNumberCriteria.empty())
		{
			bool found = (m_endpointNumberCriteria.find(pTransaction->GetEndpointNumber()) != m_endpointNumberCriteria.end());
			match &= (found == (m_endpointNumberMatchMode==matchInclude));
			matchUpdated = true;
		}

		bool filtered = false;

		if(matchUpdated)
		{
			switch(m_filterMode)
			{
			case filterAll: filtered = true; break;
			case filterMatches: filtered = match; break;
			case filterMismatches: filtered = !match; break;
			case filterNothing: filtered = false; break;
			}
		}

		if(!filtered)
		{
			SendToNextSink(pElement);
		}
	}

	virtual void FinalizeElementSink()
	{
	}
};
