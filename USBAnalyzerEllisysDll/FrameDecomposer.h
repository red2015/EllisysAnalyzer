// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// UsbFrameDecomposer.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UsbAnalysis.h"
#include "UsbAnalyzerTracker110.h"
#include "TransactionCounter.h"
#include "StringConverter.h"
#include "Statistics.h"
#include "TransactionCounter.h"

static const BYTE max_frame_value = 255;
static const size_t max_frame_bytecount = highspeed_frame_bytecount;

/////////////////////////////////////////////////////////////////////////////

class UsbFrameDecomposer : public usbdk::ChainableUsbElementSink
{
private:
	typedef std::vector<BYTE> vector_byte;

private:
	vector_byte m_frameIn;
	vector_byte m_frameOut;
	vector_byte m_frameNak;
	TransactionCounter transactionCounter;

	usbdk::usb_time m_lastSofTime;
	WORD m_sofCount;
	bool m_sofHighSpeed;

public:
	UsbFrameDecomposer() :
		m_frameIn(max_frame_bytecount, 0),
		m_frameOut(max_frame_bytecount, 0),
		m_frameNak(max_frame_bytecount, 0),
		m_lastSofTime(0.0),
		m_sofCount(0),
		m_sofHighSpeed(false)
	{
		transactionCounter = TransactionCounter();
	}

public:


	virtual void InitializeElementSink() 
	{
		Clear();
	}

	virtual void OnElementArrival(usbdk::UsbElement* pElement)
	{
		switch(pElement->GetElementType())
		{
		case usbdk::elementStartOfFrame:
			m_sofHighSpeed = ((usbdk::UsbStartOfFrame*) pElement)->GetSpeed() == usbdk::speedHigh;
			m_lastSofTime = pElement->GetTime();
			transactionCounter.IncrementTokenSOF();
			break;

		case usbdk::elementKeepAlive:
			m_sofHighSpeed = false;
			m_lastSofTime = pElement->GetTime();
			break;

		case usbdk::elementTransaction:
			{
				usbdk::UsbTransaction* transaction = (usbdk::UsbTransaction*) pElement;
				printf("\ndeviceAdd: %d", (int)transaction->GetDeviceAddress());
				IncreaseTransaction((usbdk::UsbTransaction*) pElement, 10);
				break;
			}

		case usbdk::elementSplitTransaction:
			{
				usbdk::UsbSplitTransaction* splitTransaction = (usbdk::UsbSplitTransaction*) pElement;
				printf("\nhubport: %d", (int)splitTransaction->GetSplitPacket().GetHubPort());
				IncreaseSplitTransaction((usbdk::UsbSplitTransaction*) pElement, 10);
				break;
			}

		case usbdk::elementReset:
			Clear();
			break;
		}

		SendToNextSink(pElement);
	}

	virtual void FinalizeElementSink() 
	{
	}

public:
	void DecreaseAll()
	{
		for(size_t i=0; i<max_frame_bytecount; i++)
		{
			DecreasePosition(i, m_frameIn,  DecreaseCount(m_frameIn[i]));
			DecreasePosition(i, m_frameOut, DecreaseCount(m_frameOut[i]));
			DecreasePosition(i, m_frameNak, DecreaseCount(m_frameNak[i]));
		}
	}

	void GetFrame(BYTE* pFrameIn, BYTE* pFrameOut, BYTE* pFrameNak)
	{
		CopyMemory(pFrameIn,  &m_frameIn[0],  m_frameIn.size());
		CopyMemory(pFrameOut, &m_frameOut[0], m_frameOut.size());
		CopyMemory(pFrameNak, &m_frameNak[0], m_frameNak.size());
	}

	void Clear()
	{
		m_sofCount = 0;
		m_lastSofTime = 0.0;
		ZeroMemory(&m_frameIn[0],  m_frameIn.size());
		ZeroMemory(&m_frameOut[0], m_frameOut.size());
		ZeroMemory(&m_frameNak[0], m_frameNak.size());
	}

	unsigned long GetCountTransactionsIn()
	{
		return transactionCounter.GetCountTokenIn();
	}
	unsigned long GetCountTransactionsOut()
	{
		return transactionCounter.GetCountTokenOut();
	}
	unsigned long GetCountTransactionsNak()
	{
		return transactionCounter.GetCountNAK();
	}
	unsigned long GetCountTransactionsTokenSetup()
	{
		return transactionCounter.GetCountTokenSetup();
	}
	unsigned long GetCountTransactionsTokenSOF()
	{
		return transactionCounter.GetCountTokenSOF();
	}
	unsigned long GetCountTransactionsData0()
	{
		return transactionCounter.GetCountData0();
	}
	unsigned long GetCountTransactionsData1()
	{
		return transactionCounter.GetCountData1();
	}
	unsigned long GetCountTransactionsACK()
	{
		return transactionCounter.GetCountACK();
	}

	void SetAllTrasactions(unsigned long in, unsigned long out, unsigned long setup, unsigned long sof, unsigned long data0, unsigned long data1, unsigned long nak, unsigned long ack)
	{
		transactionCounter.SetActualTransactions(in, out, setup, sof, data0, data1, nak, ack);
	}

private:
	BYTE DecreaseCount(BYTE value)
	{
		return value/25 + 3;
	}

	void IncreasePosition(size_t i, vector_byte& frame, BYTE count)
	{
		if(i > frame.size())
		{
			return;
		}

		if(frame[i] <= (max_frame_value-count))
		{
			frame[i] = frame[i] + count;
		}
		else
		{
			frame[i] = max_frame_value;
		}
	}

	void DecreasePosition(size_t i, vector_byte& frame, BYTE count)
	{
		if(i > frame.size())
		{
			return;
		}

		if(frame[i] > count)
		{
			frame[i] = frame[i] - count;
		}
		else
		{
			frame[i] = 0;
		}
	}

	void IncreaseTransaction(usbdk::UsbTransaction* pTransaction, BYTE count)
	{
		vector_byte* pFrame = NULL;

		if(pTransaction->GetHandshakePacket().GetPID() == usbdk::pidNAK)
		{
			pFrame = &m_frameNak;
			transactionCounter.IncrementNAK();
		}
		else
		{
			if(pTransaction->GetTokenPacket().GetPID() == usbdk::pidIN)
			{
				pFrame = &m_frameIn;
				transactionCounter.IncrementTokenIn();
			}
			else
			{
				pFrame = &m_frameOut;
				transactionCounter.IncrementTokenOut();
			}
			if(pTransaction->GetHandshakePacket().GetPID() == usbdk::pidACK)
			{
				transactionCounter.IncrementACK();
			}
			if(pTransaction->GetTokenPacket().GetPID() == usbdk::pidSETUP)
			{
				transactionCounter.IncrementTokenSetup();
			}
			if(pTransaction->GetTokenPacket().GetPID() == usbdk::pidSOF)
			{	
				transactionCounter.IncrementTokenSOF();
			}
			if(pTransaction->GetDataPacket().GetPID() == usbdk::pidDATA0)
			{
				transactionCounter.IncrementData0();
			}
			if(pTransaction->GetDataPacket().GetPID() == usbdk::pidDATA1)
			{
				transactionCounter.IncrementData1();
			}
		}

		ASSERT(pFrame != NULL);
		IncreasePacket(pTransaction->GetTokenPacket(), *pFrame, count);
		IncreasePacket(pTransaction->GetDataPacket(), *pFrame, count);
		IncreasePacket(pTransaction->GetHandshakePacket(), *pFrame, count);
	}

	void IncreaseSplitTransaction(usbdk::UsbSplitTransaction* pSplitTransaction, BYTE count)
	{
		vector_byte* pFrame = NULL;
		
		if(pSplitTransaction->GetHandshakePacket().GetPID() == usbdk::pidNAK)
		{
			pFrame = &m_frameNak;
			transactionCounter.IncrementNAK();
		}
		else
		{
			if(pSplitTransaction->GetTokenPacket().GetPID() == usbdk::pidIN)
			{
				pFrame = &m_frameIn;
				transactionCounter.IncrementTokenIn();
			}
			else
			{
				pFrame = &m_frameOut;
				transactionCounter.IncrementTokenOut();
			}
			if(pSplitTransaction->GetHandshakePacket().GetPID() == usbdk::pidACK)
			{
				transactionCounter.IncrementACK();
			}
			if(pSplitTransaction->GetTokenPacket().GetPID() == usbdk::pidSETUP)
			{
				transactionCounter.GetCountTokenSetup();
			}
			if(pSplitTransaction->GetTokenPacket().GetPID() == usbdk::pidSOF)
			{	
				transactionCounter.IncrementTokenSOF();
			}
			if(pSplitTransaction->GetDataPacket().GetPID() == usbdk::pidDATA0)
			{
				transactionCounter.IncrementData0();
			}
			if(pSplitTransaction->GetDataPacket().GetPID() == usbdk::pidDATA1)
			{
				transactionCounter.IncrementData1();
			}
			
			
		}

		ASSERT(pFrame != NULL);
		IncreasePacket(pSplitTransaction->GetSplitPacket(), *pFrame, count);
		IncreasePacket(pSplitTransaction->GetTokenPacket(), *pFrame, count);
		IncreasePacket(pSplitTransaction->GetDataPacket(), *pFrame, count);
		IncreasePacket(pSplitTransaction->GetHandshakePacket(), *pFrame, count);
	}

	void IncreasePacket(const usbdk::UsbPacket& packet, vector_byte& frame, BYTE count)
	{
		if(packet.IsEmpty())
		{
			return;
		}

		usbdk::usb_time frameTime = packet.GetTime() - m_lastSofTime;
		size_t framePosition = TimeToByteCount(frameTime, max_frame_bytecount, FrameDuration(packet.GetSpeed()));
		size_t packetSize = GetHighSpeedPacketByteCount(packet);

		for(size_t i=0; i<packetSize; i++)
		{
			IncreasePosition(framePosition + i, frame, count);
		}
	}

	size_t GetHighSpeedPacketByteCount(const usbdk::UsbPacket& packet)
	{
		size_t count = GetPacketByteCount(packet);

		C_ASSERT(usbdk::speedCount == 4);
		switch(packet.GetSpeed())
		{
		case usbdk::speedHigh:
			break;

		case usbdk::speedFull:
			count *= 5;
			break;

		case usbdk::speedLow:
		case usbdk::speedLowPrefixed:
			count *= 40;
			break;
		}

		return count;
	}
};
