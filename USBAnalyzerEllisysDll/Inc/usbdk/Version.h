// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file Version.h
/// @brief
///		USB Analysis SDK version declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/// @brief
/// 	Represents a version number.
class Version
{
private:
	DWORDLONG m_version;

public:
	/// @brief
	/// 	Initializes a new instance of the Version class.
	Version();

	/// @brief
	/// 	Initializes a new instance of the Version class using the value represented by the specified string.
	/// @param
	/// 	version - A string containing the major, minor, build, and revision numbers, where each number is delimited with a period character ('.').
	/// @remarks
	/// 	The version parameter can only contain components major, minor, build, and revision, 
	/// 	in that order, and all separated by periods. There must be at least two components, 
	/// 	and at most four. The first two components are assumed to be major and minor. 
	/// 	The value of unspecified components is zero.
	/// 	For example, if the major number is 1, the minor number is 10, the build number is 1700, 
	/// 	and the revision number is 0, then version should be "1.10.1700.0".
	Version(const std::tstring& version);

	/// @brief
	/// 	Initializes a new instance of the Version class using the specified major and minor values.
	/// @param major - The major version number. 
	/// @param minor - The minor version number. 
	Version(WORD major, WORD minor);

	/// @brief
	/// 	Initializes a new instance of the Version class using the specified major, minor and build values.
	/// @param major - The major version number. 
	/// @param minor - The minor version number. 
	/// @param build - The build number. 
	Version(WORD major, WORD minor, WORD build);

	/// @brief
	/// 	Initializes a new instance of the Version class using the specified value.
	/// @param version - The version number. 
	Version(DWORDLONG version);

public:
	/// Gets the value of the major component of the version number for this instance.
	WORD GetMajor() const;

	/// Gets the value of the minor component of the version number for this instance.
	WORD GetMinor() const;

	/// Gets the value of the build component of the version number for this instance.
	WORD GetBuild() const;

public:
	/// @brief
	/// 	Converts the value of this instance to its equivalent string representation.
	std::tstring Format() const;

	/// @brief
	/// 	Converts the value of this instance to its equivalent string representation.
	/// 	A specified count indicates the number of components to return.
	/// @param
	/// 	fieldCount - The number of components to return. The fieldCount ranges from 0 to 4. 
	std::tstring Format(int fieldCount) const;

public:
	/// Cast a version instance to DWORDLONG.
	operator DWORDLONG() const;
};

/// @brief
/// 	Contains information about the development kit.
class DevelopmentKitInformation
{
private:
	// Ensure that no object of this class can be instanciated
	virtual ~DevelopmentKitInformation() = 0;

public:
	/// @brief
	/// 	Returns the version of the development kit.
	/// @seealso
	/// 	Version
	static Version GetVersion();
};

} // End of the usbdk namespace
