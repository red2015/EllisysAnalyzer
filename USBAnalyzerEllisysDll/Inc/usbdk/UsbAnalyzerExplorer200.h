// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

#pragma once

extern usbdk::IUsbAnalyzerFactory* UsbExplorer200_CreateAnalyzerFactory();
extern void UsbExplorer200_RegisterAnalyzerFactory(usbdk::UsbAnalyzerFactoryManager* pFactoryManager);
