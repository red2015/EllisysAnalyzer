// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

#pragma once

std::tstring FormatPID(usbdk::usb_pid pid);
std::tstring FormatTokenPID(usbdk::usb_pid tokenPid);
std::tstring FormatDataPID(usbdk::usb_pid dataPid);
std::tstring FormatHandshakePID(usbdk::usb_pid handshakePid);
std::tstring FormatBusState(usbdk::usb_element_type busState);
std::tstring FormatSubPID(usbdk::usb_subpid subpid);
std::tstring FormatSpeed(usbdk::usb_speed speed);
std::tstring FormatDeviceAddress(usbdk::usb_device_address deviceAddress);
std::tstring FormatDeviceAddress(usbdk::usb_device_address deviceAddress, usbdk::usb_device_address deviceAddressPostEnum);
std::tstring FormatEndpointNumber(usbdk::usb_endpoint_number endpointNumber);
std::tstring FormatFrameNumber(usbdk::usb_frame_number frame, bool shortText);
std::tstring FormatMicroFrameNumber(usbdk::usb_microframe_number microFrame, bool shortText);
std::tstring FormatSplitType(usbdk::usb_split_type type);
std::tstring FormatSplitStartComplete(bool sc);
std::tstring FormatSplitEndpointType(BYTE endpointType);
std::tstring FormatLpmLinkState(WORD attributes);
std::tstring FormatLpmRemoteWake(WORD attributes);
std::tstring FormatTime(usbdk::usb_time time);
std::tstring FormatColumnDataText(const usbdk::vector_usbdata& data);
bool IsTimeReferenceReset();
void ResetTimeReference();
usbdk::usb_time GetTimeReference();
void SetTimeReference(usbdk::usb_time referenceTime);
