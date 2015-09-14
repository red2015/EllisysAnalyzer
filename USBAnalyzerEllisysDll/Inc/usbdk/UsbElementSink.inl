// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "UsbElementSink.h"

//////////////////////////////////////////////////////////////////////

namespace usbdk
{

void ChainableUsbElementSink::SetNextSink(IUsbElementSink* pNextSink)
{
	m_pNextSink = pNextSink;
}

const IUsbElementSink* ChainableUsbElementSink::GetNextSink() const
{
	return m_pNextSink;
}

IUsbElementSink* ChainableUsbElementSink::GetNextSink()
{
	return m_pNextSink;
}

void ChainableUsbElementSink::SendToNextSink(UsbElement* pElement)
{
	if(m_pNextSink != NULL)
	{
		m_pNextSink->OnElementArrival(pElement);
	}
}

bool ChainableUsbElementSink::IsLastSink() const
{
	return (m_pNextSink == NULL);
}

}