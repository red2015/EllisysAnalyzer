// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

#pragma once

extern usbdk::IUsbAnalyzerFactory* UsbTracker110_CreateAnalyzerFactory();
extern void UsbTracker110_RegisterAnalyzerFactory(usbdk::UsbAnalyzerFactoryManager* pFactoryManager);
