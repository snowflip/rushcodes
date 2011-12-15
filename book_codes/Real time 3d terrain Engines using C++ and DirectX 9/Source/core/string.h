#ifndef STRING_H
#define STRING_H
#pragma once
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    sample code from the book...
    Real Time 3D Terrain Engines Using C++ and DirectX

    by Greg Snook
    greg@mightystudios.com

\***************************************************************/
#include <stdio.h> 
#include <stdarg.h> 
#include <tchar.h> 

#ifndef STD_STRING_H
#define STD_STRING_H
#pragma warning(disable : 4786)
#include <string>
#endif

#ifndef DATATYPES_H
#include "data_types.h"
#endif

//	Name Space declaration
namespace gaia
{

/*	cString
------------------------------------------------------------------------------------------
	
	A cString (aka "cString String") is a wrapper around the STL string template class. cString
	provides transparent access to string functions with some added functionality.
	
------------------------------------------------------------------------------------------
*/

inline void FormatText(tchar* buffer, const tchar* text, va_list ArgList)
{
#ifdef _UNICODE
	vswprintf(buffer, text, ArgList);
#else
	vsprintf(buffer, text, ArgList);
#endif
}

inline void __cdecl FormatText(tchar* buffer, const tchar* text, ...)
{
	va_list		ArgList;
	va_start(ArgList, text);
	FormatText(buffer, text, ArgList);
	va_end(ArgList);	
}


typedef std::basic_string<tchar, 
	std::char_traits<tchar> > TEXT_BASE_CLASS;

class cString: public TEXT_BASE_CLASS
{
public:

	// Creators...
	inline cString(){};
	cString(const cString& rhs);
	explicit cString(const TEXT_BASE_CLASS& rhs);
	explicit cString(tchar c);
	cString(const tchar *s, ...);
	~cString(){};

	// cast the string directly to a tchar pointer
	inline operator const tchar*()const;

	// copy data from various source types
	cString& operator=(const cString& rhs);
	cString& operator=(const TEXT_BASE_CLASS& rhs);
    cString& operator=(const tchar *s);
    cString& operator=(tchar c);

	// append data from various sources
    cString& operator+=(const cString& rhs);
    cString& operator+=(const TEXT_BASE_CLASS& rhs);
    cString& operator+=(const tchar *s); 
	cString& operator+=(tchar c);

	// test equality and inequality (case sensitive)
	bool operator==(const cString& rhs);
	bool operator!=(const cString& rhs);

	// test without case sensitivity
	bool compareWithoutCase(const cString& rhs);

	// operator that allows cString to be used as an STL map key value
	bool operator<(const cString& rhs);

	// format the string from optional parameters
	void __cdecl format(const tchar* text, ...);
	void __cdecl format(const tchar* text, va_list arglist);

	void removeFileExtension(cString* pExtensionFound=0);
};

//- Inline Functions ---------------------------------------------------------------------

//. Creators .............................................................................

inline cString::cString(const cString& rhs)
{
	assign((const TEXT_BASE_CLASS) rhs); 
}

inline cString::cString(const TEXT_BASE_CLASS& rhs)
{
	assign((const TEXT_BASE_CLASS) rhs); 
}

inline cString::cString(const tchar *s, ...)
{
	va_list		arglist;
	va_start(arglist, s);

	if (*arglist != 0)
	{
		format(s, arglist);
	}
	else
	{
		assign(s);
	}

	va_end(arglist);	
}

inline cString::cString(tchar c)
{
	assign(1,c); 
}

//. Operators ............................................................................
inline bool cString::operator==(const cString& rhs)
{
	return (compare(rhs) == 0);
}

inline bool cString::operator!=(const cString& rhs)
{
	return (compare(rhs) != 0);
}

inline cString& cString::operator=(const cString& rhs) 
{
	assign((const TEXT_BASE_CLASS) rhs); 
	return (*this);
}

inline cString& cString::operator=(const TEXT_BASE_CLASS& rhs) 
{
	assign((const TEXT_BASE_CLASS) rhs); 
	return (*this);
}

inline cString& cString::operator=(const tchar *s)
{
	assign(s); 
	return (*this);
}

inline cString& cString::operator=(tchar c)
{
	assign(1,c); 
	return (*this);
}

inline cString& cString::operator+=(const cString& rhs)
{
	append((const TEXT_BASE_CLASS) rhs); 
	return (*this);
}

inline cString& cString::operator+=(const TEXT_BASE_CLASS& rhs)
{
	append((const TEXT_BASE_CLASS) rhs); 
	return (*this);
}

inline cString& cString::operator+=(const tchar *s)
{
	append(s); 
	return (*this);
}
 
inline cString& cString::operator+=(tchar c)
{
	append(1,c); 
	return (*this);
}


//. Helper Functions .....................................................................


inline cString operator+(const cString& lhs, const cString& rhs)
{
	cString NewText(lhs);
	NewText+=rhs;
	return(NewText);
}

inline cString::operator const tchar*()const
{
	return (c_str());
}

inline bool cString::compareWithoutCase(const cString& rhs)
{
	return(_tcsicmp(c_str(), rhs.c_str()) == 0);
}

inline bool cString::operator<(const cString& rhs)
{
	return(strcmp(c_str(), rhs.c_str()) < 0);
};

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cString.h )

//----------------------------------------------------------
//$Log: $