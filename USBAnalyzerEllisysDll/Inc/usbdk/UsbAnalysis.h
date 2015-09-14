// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file UsbAnalysis.h
/// @brief
///		USB Analysis SDK main header.
/////////////////////////////////////////////////////////////////////////////

#pragma once

/// @brief 
/// 	USB Analysis SDK namespace.
/// @remarks
/// 	This namespace contains all the declarations of the interfaces and 
/// 	the objects used by the development kit.
namespace usbdk { }

#include "StandardEllisys.h"
#include "SimpleGuid.h"
#include "RefCount.h"
#include "UsbDefs.h"
#include "UsbTypes.h"
#include "UsbCrc.h"
#include "UsbPackets.h"
#include "UsbElements.h"
#include "UsbElementFactory.h"
#include "UsbElementSink.h"
#include "UsbElementInjector.h"
#include "UsbAnalyzer.h"
#include "Version.h"

//---------------------------------------------------------------
// Automatic library include
//---------------------------------------------------------------

#if defined _LIB && !defined USBDK_NOAUTOLINK
	#define USBDK_NOAUTOLINK
#endif

#ifndef USBDK_NOAUTOLINK
	#ifndef _MSC_VER
		#error The USB Analysis SDK supports only Microsoft Visual C++ compilers.
	#else
		#if (_MSC_VER<1400)
			#error The USB Analysis SDK supports only Microsoft Visual C++ 2005 or higher.

		// Microsoft Visual C++ 2005
		#elif (_MSC_VER>=1400 && _MSC_VER<1401)
			#ifdef _DEBUG
				#ifdef _WIN64
					#ifdef _UNICODE
						#pragma comment(lib, "usbdk80x64ud")
					#else
						#pragma comment(lib, "usbdk80x64d")
					#endif
				#else
					#ifdef _UNICODE
						#pragma comment(lib, "usbdk80ud")
					#else
						#pragma comment(lib, "usbdk80d")
					#endif
				#endif
			#else
				#ifdef _WIN64
					#ifdef _UNICODE
						#pragma comment(lib, "usbdk80x64u")
					#else
						#pragma comment(lib, "usbdk80x64")
					#endif
				#else
					#ifdef _UNICODE
						#pragma comment(lib, "usbdk80u")
					#else
						#pragma comment(lib, "usbdk80")
					#endif
				#endif
			#endif

		#else
			#error This version of Microsoft Visual C++ is not supported. Please download the latest version of the SDK on http://www.ellisys.com/products/usbsdk.
		#endif
	#endif
#endif
