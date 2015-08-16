// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// SimpleGuid.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#endif // GUID_DEFINED
