#define CSTRING_CPP
#define CORE_DLL
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

#include "string.h"

using namespace gaia;

void __cdecl cString::format(const tchar* text, ...)
{
	va_list		arglist;
	va_start(arglist, text);

	format(text, arglist);
	va_end(arglist);	
}

void __cdecl cString::format(const tchar* text, va_list arglist)
{
	const	int		TEXT_BUFFERSIZE = 2048;
	tchar	text_buffer[TEXT_BUFFERSIZE];

	FormatText(text_buffer, text, arglist);

	(*this) = text_buffer; 
}

void cString::removeFileExtension(cString* pExtensionFound)
{
	size_t length = size();
	size_t extensionStart = find_last_of('.');

	if (extensionStart >=0 && extensionStart<length)
	{
		if (pExtensionFound)
		{
			*pExtensionFound = substr(extensionStart+1, length-extensionStart-1);
		}

		*this = substr(0, extensionStart);
	}
}

//***************************************************************
// end of file      ( cString.cpp )

//----------------------------------------------------------
//$Log: $