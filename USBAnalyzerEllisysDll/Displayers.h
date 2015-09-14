// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// Displayers.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include "UsbAnalysis.h"
#include "UsbFormatters.h"

/////////////////////////////////////////////////////////////////////////////

inline void DisplayAvailableUsbAnalyzers(usbdk::vector_usb_analyzer_information& analyzers)
{
	_tprintf(_T("Available USB Analyzers\n"));
	_tprintf(_T("-----------------------\n"));

	for(usbdk::vector_usb_analyzer_information::const_iterator pAnalyzer=analyzers.begin(); pAnalyzer!=analyzers.end(); pAnalyzer++)
	{
		_tprintf(_T("%s %s - %s\n"), 
			pAnalyzer->GetManufacturer().c_str(), 
			pAnalyzer->GetModelName().c_str(), 
			pAnalyzer->GetSerialNumber().c_str());
	}

	_tprintf(_T("\n"));
}

class ConsoleDisplayer : public usbdk::UsbElementProcessor
{
private:
	bool m_displayTime;
	bool m_displayEnabled;
	size_t m_maxDataToDisplay;

public:
	ConsoleDisplayer() :
		usbdk::UsbElementProcessor(),
		m_displayTime(true),
		m_displayEnabled(true),
		m_maxDataToDisplay((size_t) -1)
	{
	}

public:
	void EnableDisplay(bool enable)
	{
		m_displayEnabled = enable;
	}

	void DisplayTime(bool display)
	{
		m_displayTime = display;
	}

	void SetMaxDataToDisplay(size_t cb)
	{
		m_maxDataToDisplay = cb;
	}

public:
	virtual void InitializeElementSink()
	{
	}

	virtual void OnElementArrival(usbdk::UsbElement* pElement)
	{
		if(m_displayEnabled)
		{
			if(m_displayTime)
			{
				TCHAR buffer[16];
					
				_stprintf_s(buffer, countof(buffer), _T("%9.6lf  "), pElement->GetTime());		
				std::tcout << buffer;
				//Sleep(100);
			}

			UsbElementProcessor::OnElementArrival(pElement);
		}
		else
		{
			SendToNextSink(pElement);
		}
	}

	virtual void FinalizeElementSink()
	{
	}

protected:
	virtual void ProcessInvalidPacket(usbdk::UsbInvalidPacket* pInvalidPacket)
	{
		std::tcout << _T("Invalid Packet rawData=");
		DumpData(pInvalidPacket->GetPacket().GetRawData());
		std::tcout << std::endl;
	}

	virtual void ProcessStartOfFrame(usbdk::UsbStartOfFrame* pStartOfFrame)
	{
		std::tcout	<< _T("Start-of-Frame #") 
					<< pStartOfFrame->GetPacket().GetFrameNumber()
					<< std::endl;
	}

	virtual void ProcessTransaction(usbdk::UsbTransaction* pTransaction)
	{
		if(!pTransaction->IsValid())
		{
			std::tcout << _T("!! ");
		}

		if(!pTransaction->GetTokenPacket().IsEmpty())
		{
			TCHAR buffer[32];
			_stprintf_s(buffer, countof(buffer), _T("%-6s"), FormatPID(pTransaction->GetTokenPacket().GetPID()).c_str());

			std::tcout	<< buffer
						<< _T(" dev=") << (int) pTransaction->GetDeviceAddress()
						<< _T(" ep=") << (int) pTransaction->GetEndpointNumber()
						<< _T("  ");
		}

		if(!pTransaction->GetDataPacket().IsEmpty())
		{
			std::tcout	<< FormatPID(pTransaction->GetDataPacket().GetPID()).c_str() << _T(" ");

			const usbdk::vector_usbdata& content = pTransaction->GetData();
			if(content.size() > 0)
			{
				std::tcout	<< _T("size=") << (int) content.size()
							<< _T(" data=");

				DumpData(content, m_maxDataToDisplay);
			}
			else
			{
				std::tcout << _T("no data ");
			}

			std::tcout << _T(" ");
		}

		if(!pTransaction->GetHandshakePacket().IsEmpty())
		{
			std::tcout << FormatPID(pTransaction->GetHandshakePacket().GetPID()).c_str();
		}

		std::tcout << std::endl;
	}

	virtual void ProcessSplitTransaction(usbdk::UsbSplitTransaction* pSplitTransaction)
	{
		if(!pSplitTransaction->GetSplitPacket().IsEmpty())
		{
			std::tstring endpointType;

			C_ASSERT(usbdk::splitEndpointTypeCount == 4);
			switch(pSplitTransaction->GetSplitPacket().GetEndpointType())
			{
			case usbdk::splitEndpointTypeControl:		endpointType = _T("CTL"); break;
			case usbdk::splitEndpointTypeIsochronous:	endpointType = _T("ISO"); break;
			case usbdk::splitEndpointTypeBulk:			endpointType = _T("BULK"); break;
			case usbdk::splitEndpointTypeInterrupt:		endpointType = _T("INT"); break;
			case usbdk::splitEndpointTypeUnknown:		endpointType = _T("Unknown"); break;
			}

			std::tcout	<< _T("SPLIT") 
						<< _T(" HubAddr=")	<< (int) pSplitTransaction->GetSplitPacket().GetHubAddress()
						<< _T(" SC=")		<< (int) pSplitTransaction->GetSplitPacket().GetStartComplete()
						<< _T(" Port=")		<< (int) pSplitTransaction->GetSplitPacket().GetHubPort()
						<< _T(" S=")		<< (int) pSplitTransaction->GetSplitPacket().GetS()
						<< _T(" E=")		<< (int) pSplitTransaction->GetSplitPacket().GetE()
						<< _T(" ET=")		<< endpointType.c_str()
						<< std::endl		<< _T("           ");
		}

		if(!pSplitTransaction->GetTokenPacket().IsEmpty())
		{
			TCHAR buffer[16];
			_stprintf_s(buffer, countof(buffer), _T("%-6s"), FormatSubPID(pSplitTransaction->GetTokenPacket().GetPID()).c_str());

			std::tcout	<< buffer
						<< _T(" dev=")	<< (int) pSplitTransaction->GetTokenPacket().GetDeviceAddress()
						<< _T(" ep=")	<< (int) pSplitTransaction->GetTokenPacket().GetEndpointNumber()
						<< _T("  ");
		}

		if(!pSplitTransaction->GetDataPacket().IsEmpty())
		{
			std::tcout	<< FormatPID(pSplitTransaction->GetDataPacket().GetPID()).c_str() << _T(" ");

			const usbdk::vector_usbdata& content = pSplitTransaction->GetDataPacket().GetData();

			if(content.size() > 0)
			{
				std::tcout	<< _T("size=") << (int) content.size()
							<< _T(" data=");

				DumpData(content, m_maxDataToDisplay);
			}
			else
			{
				std::tcout << _T("no data ");
			}

			std::tcout << _T(" ");
		}

		if(!pSplitTransaction->GetHandshakePacket().IsEmpty())
		{
			std::tcout << FormatPID(pSplitTransaction->GetHandshakePacket().GetPID()).c_str();
		}

		std::tcout << std::endl;
	}

	virtual void ProcessLpmTransaction(usbdk::UsbLpmTransaction* pLpmTransaction)
	{
		if(!pLpmTransaction->IsValid())
		{
			std::tcout << _T("!! ");
		}

		if(!pLpmTransaction->GetTokenPacket().IsEmpty())
		{
			TCHAR buffer[16];
			_stprintf_s(buffer, countof(buffer), _T("%-6s"), FormatPID(pLpmTransaction->GetTokenPacket().GetPID()).c_str());

			std::tcout	<< buffer
						<< _T(" dev=") << (int) pLpmTransaction->GetDeviceAddress()
						<< _T(" ep=") << (int) pLpmTransaction->GetEndpointNumber()
						<< _T("  ");
		}

		if(!pLpmTransaction->GetExtTokenPacket().IsEmpty())
		{
			TCHAR buffer[16];
			_stprintf_s(buffer, countof(buffer), _T("%-6s"), FormatPID(pLpmTransaction->GetExtTokenPacket().GetSubPID()).c_str());
			WORD extTokenPayload = pLpmTransaction->GetExtTokenPacket().GetPayload();

			std::tcout	<< buffer
						<< _T(" linkState=L") << (int) (extTokenPayload & 0xF)
						<< _T(" remoteWake=") << ((extTokenPayload & 0x100) ? _T("enable") : _T("disable"))
						<< _T("  ");
		}

		if(!pLpmTransaction->GetHandshakePacket().IsEmpty())
		{
			std::tcout << FormatPID(pLpmTransaction->GetHandshakePacket().GetPID()).c_str();
		}

		std::tcout << std::endl;
	}

	virtual void ProcessKeepAlive(usbdk::UsbKeepAlive* /*pKeepAlive*/)
	{
		std::tcout << _T("Keep Alive") << std::endl;
	}

	virtual void ProcessReset(usbdk::UsbReset* pReset)
	{
		std::tcout << _T("Extended SE0  duration=") << pReset->GetDuration() << std::endl;
	}

	virtual void ProcessSuspended(usbdk::UsbSuspended* pSuspended)
	{
		std::tcout << _T("Suspended   duration=") << pSuspended->GetDuration() << std::endl;
	}

	virtual void ProcessPowerChange(usbdk::UsbPowerChange* pPowerChange)
	{
		std::tstring str = (pPowerChange->GetPowerChange() == usbdk::powerChangeOn) ? _T("Power ON") : _T("Power OFF");
		std::tcout << str << std::endl;
	}

	virtual void ProcessHighSpeedHandshake(usbdk::UsbHighSpeedHandshake* pHighSpeedHandshake)
	{
		std::tstring status;

		C_ASSERT(usbdk::hsHandshakeCount == 4);
		switch(pHighSpeedHandshake->GetStatus())
		{
		case usbdk::hsHandshakeSuccess:
			status = _T("OK");
			break;

		case usbdk::hsHandshakeDownstreamPortChirpKTimeout:
			status = _T("DOWNSTREAM TIMEOUT");
			break;

		case usbdk::hsHandshakeUpstreamPortChirpKTimeout:
			status = _T("UPSTREAM TIMEOUT");
			break;

		case usbdk::hsHandshakeNotEnoughDownstreamChirps:
			status = _T("ERROR");
			break;
		}

		std::tcout	<< _T("High Speed Handshake   status=")
					<< status
					<< std::endl;
	}

	virtual void ProcessTrigger(usbdk::UsbTrigger* pTrigger)
	{
		std::tcout	<< _T("Hardware Trigger Event")
					<< std::endl;
	}

	virtual void ProcessUnknownElement(usbdk::UsbElement* /*pElement*/)
	{
		std::cout << "Unknown element" << std::endl;
	}

private:
	template<class Container>
	void DumpData(const Container& content, size_t cbToDisplay=-1)
	{
		size_t cbDisplayed = min(content.size(), cbToDisplay);

		TCHAR buffer[8];
		for(size_t i=0; i<cbDisplayed; i++)
		{
			_stprintf_s(buffer, countof(buffer), _T("%02X "), content[i]);
			std::tcout << buffer;
		}

		if(cbDisplayed < content.size())
		{
			std::tcout << _T("...");
		}
	}
};
