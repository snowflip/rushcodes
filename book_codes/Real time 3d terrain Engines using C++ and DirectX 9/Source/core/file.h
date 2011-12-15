#ifndef FILE_H
#define FILE_H
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

#ifndef STRING_H
#include "string.h"
#endif


//	Name Space declaration
namespace gaia
{


/*	cFile
------------------------------------------------------------------------------------------
	
	Basic File IO routines
	
------------------------------------------------------------------------------------------
*/

class cFile
{
public:

	// data Types & Constants...
	enum FILE_ACCESS
	{
		READ_ONLY,
		WRITE_ONLY,
		READ_WRITE,
		APPEND_ONLY,
		READ_AND_APPEND
	};

	// Creators...
	cFile();
	cFile(const cString& FileName, uint32 AccessRights=READ_WRITE);
	~cFile();

	// file creation
	bool open();
	bool open(const cString& FileName, uint32 AccessRights=READ_WRITE);
	bool create();
	bool create(const cString& FileName);
	void close();

	// push/pull operators...
	cFile& operator>>(int8& data);
	cFile& operator>>(uint8& data);
	cFile& operator>>(int16& data);
	cFile& operator>>(uint16& data);
	cFile& operator>>(int32& data);
	cFile& operator>>(uint32& data);
	cFile& operator>>(float& data);
	cFile& operator>>(bool& data);

	cFile& operator<<(int8 data);
	cFile& operator<<(uint8 data);
	cFile& operator<<(int16 data);
	cFile& operator<<(uint16 data);
	cFile& operator<<(int32 data);
	cFile& operator<<(uint32 data);
	cFile& operator<<(float data);
	cFile& operator<<(bool data);

	// writing functions
	bool write(const void *Src, uint32 BytesToWrite);
	bool write(const cString& string, uint32* pBytesWritten=0);
	bool write(int8 Value);
	bool write(uint8 Value);
	bool write(int16 Value);
	bool write(uint16 Value);
	bool write(int32 Value);
	bool write(uint32 Value);
	bool write(float Value);
	bool write(bool Value);

	// reading functions
	bool read(void *Dest, uint32 BytesToRead);
	bool read(cString& string);
	bool read(int8& Value);
	bool read(uint8& Value);
	bool read(int16& Value);
	bool read(uint16& Value);
	bool read(int32& Value);
	bool read(uint32& Value);
	bool read(float& Value);
	bool read(bool& Value);

	// file position
	uint32 position();
	bool setPosition(uint32 NewPos);
	bool seek(long Offset);

	// accessors...
	bool isFileOpen();
	const cString& name();
	uint32 size();

private:

	// data...
	uint32 m_fileAccess;
	void *m_fileHandle;
	cString m_fileName;

	// Nonexistant Functions...
	cFile( const cFile&  Src);
	cFile& operator=( const cFile& Src);
	bool operator == ( const cFile&  Src) const;
};

//- Inline Functions ---------------------------------------------------------------------

//. Creators .............................................................................
inline cFile::cFile()
:m_fileHandle(0)
{
	m_fileName.erase();
}

inline cFile::cFile(const cString& FileName, uint32 AccessRights)
:m_fileHandle(0)
,m_fileName(FileName)
,m_fileAccess(AccessRights)
{
}

inline cFile::~cFile()
{
	close();
}

//. Operators ............................................................................

inline cFile& cFile::operator>>(int8& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(uint8& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(int16& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(uint16& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(int32& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(uint32& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(float& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator>>(bool& data)
{
	read((void*)&data, sizeof(data));
	return(*this);
}


inline cFile& cFile::operator<<(int8 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(uint8 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(int16 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(uint16 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(int32 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(uint32 data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(float data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}

inline cFile& cFile::operator<<(bool data)
{
	write((void*)&data, sizeof(data));
	return(*this);
}


//. Accessors ............................................................................

inline bool cFile::isFileOpen()
{
	return (m_fileHandle ? true : false);
}

inline const cString& cFile::name()
{
	return (m_fileName);
}


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cFile.h )

//----------------------------------------------------------
//$Log: $
