#ifndef DEBUGCHANNEL_H
#define DEBUGCHANNEL_H
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
#ifndef DATATYPES_H
#include "data_types.h"
#endif

#ifndef NUMERICTOOLS_H
#include "numeric_tools.h"
#endif

#ifndef STRING_H
#include "string.h"
#endif

#ifndef FILE_H
#include "file.h"
#endif

//	Name Space declaration
namespace gaia
{

// forward declaration of cDebugMessageHandler
class cDebugMessageHandler;

/*	cDebugMessageChannel
------------------------------------------------------------------------------------------
	
	The virtual base class for cDebugMessageHandler Output objects. The write() function must be
	defined by the class. Anything which will output debug messages should be derived
	from this class. Open and Close are provided for those channels which require
	innitializtion, such as opening and closing log files.
	
------------------------------------------------------------------------------------------
*/
class cDebugMessageChannel
{
public:

	// a public set of bitflags used to filter messages
	u32Flags messageFilter;

	cDebugMessageChannel():messageFilter(0){}
	virtual ~cDebugMessageChannel(){}

private:

	// these functions are only called by the cDebugMessageHandler object
	friend cDebugMessageHandler;
	virtual bool open(){return true;}
	virtual void close(){}
	virtual bool write(const tchar* pText) = 0;

};

/*	cDefaultOutputChannel
------------------------------------------------------------------------------------------
	
	A specific type of cDebugMessageChannel designed to recieve all messages and output them
	to the debug console via printf()
	
------------------------------------------------------------------------------------------
*/
class cDefaultOutputChannel : public cDebugMessageChannel
{
public:

	cDefaultOutputChannel()
	{
		// accept all message types
		messageFilter = 0xffffffff;
	}
	~cDefaultOutputChannel(){}

private:

	// output all text to the standard output buffer
	bool write(const tchar* pText);
};

/*	cFileOutputChannel
------------------------------------------------------------------------------------------
	
	A Debug Channel for standard output to a log file
	
------------------------------------------------------------------------------------------
*/
class cFileOutputChannel : public cDebugMessageChannel
{
public:

	cFileOutputChannel(const tchar* pFileName=0, uint32 AccessRights=cFile::READ_WRITE)
	:m_file(pFileName, AccessRights)
	{};

	~cFileOutputChannel(){}

	// Accessors...
	cFile& file() {return m_file;}

private:

	bool open();
	void close();
	bool write(const tchar* pText);

	cFile m_file;
};


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( debug_channel.h )

//----------------------------------------------------------
//$Log: $