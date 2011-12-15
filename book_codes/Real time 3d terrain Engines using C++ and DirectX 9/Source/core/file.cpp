#define CFILE_CPP
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

#include "file.h"


#include <stdio.h>
#include <conio.h>
#include "debug.h"

using namespace gaia;

bool cFile::open()
{
	debug_assert(m_fileName.size() != 0, "No filename provided");

	return open(m_fileName, m_fileAccess);
}

bool cFile::open(const cString& file_name, uint32 access_rights)
{
	char file_access[3];

	close();

	switch(access_rights)
	{
		case READ_ONLY:
			strcpy(file_access,"rb");
			break;
		case WRITE_ONLY:
			strcpy(file_access,"wb");
			break;
		case APPEND_ONLY:
			strcpy(file_access,"ab");
			break;
		case READ_AND_APPEND:
			strcpy(file_access,"a+b");
			break;

		default:
		case READ_WRITE:
			strcpy(file_access,"r+b");
			break;

	}

	m_fileHandle=(void*)fopen((const char*)file_name,file_access);
	
	if(m_fileHandle==0)
	{
		return (false);
	}

	m_fileAccess=access_rights;
	m_fileName=file_name;

	return (true);
}

bool cFile::create()
{
	return open(m_fileName);
}

bool cFile::create(const cString& file_name)
{
	close();

	m_fileHandle=(void*)fopen((const char*)file_name,"w+b");
	
	if(m_fileHandle==0)
	{
		return (false);
	}

	m_fileAccess=READ_WRITE;
	m_fileName=file_name;

	return (true);
}

void cFile::close()
{
	if (m_fileHandle)
	{
		fflush((FILE*)m_fileHandle);
		fclose((FILE*)m_fileHandle);
		m_fileHandle = 0;
	}
}

bool cFile::read(void *Dest, uint32 BytesToRead)
{
	unsigned long BytesRead = 0;

	debug_assert(m_fileHandle, "There is no file handle open");

	BytesRead = fread(Dest, 1, BytesToRead, (FILE*)m_fileHandle);

	return (BytesRead == BytesToRead);
}

bool cFile::write(const void *Src, uint32 BytesToWrite)
{
	unsigned long BytesWritten = 0;

	debug_assert(m_fileHandle, "There is no file handle open");

	if (BytesToWrite && Src)
	{
		BytesWritten = fwrite(Src, 1, BytesToWrite, (FILE*)m_fileHandle);
	}
	return (BytesWritten == BytesToWrite);
}

bool cFile::write(int32 value)
{
	return(write(&value, sizeof(int32)));
}

bool cFile::write(float value)
{
	return(write(&value, sizeof(float)));
}

bool cFile::write(uint32 value)
{
	return(write(&value, sizeof(uint32)));
}

bool cFile::write(bool value)
{
	return(write(&value, sizeof(bool)));
}


bool cFile::read(int32& value)
{
	return(read(&value, sizeof(int)));
}

bool cFile::read(float& value)
{
	return(read(&value, sizeof(float)));
}

bool cFile::read(uint32& value)
{
	return(read(&value, sizeof(uint32)));
}

bool cFile::read(bool& value)
{
	return(read(&value, sizeof(bool)));
}


uint32 cFile::position()
{
	debug_assert(m_fileHandle, "There is no file handle open");

	return (ftell((FILE*)m_fileHandle));
}

bool cFile::setPosition(uint32 NewPos)
{
	debug_assert(m_fileHandle, "There is no file handle open");

	if(!fseek((FILE*)m_fileHandle, (long)NewPos, SEEK_SET))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool cFile::seek(long Offset)
{
	debug_assert(m_fileHandle, "There is no file handle open");

	if(!fseek((FILE*)m_fileHandle, (long)Offset, SEEK_CUR))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool cFile::write(const cString& string, uint32* pBytesWritten)
{
	char* name =0;
	int32 StringLength=string.size()+1;

	if (write(StringLength))
	{
		if (!StringLength
			||write((void*)string.c_str(), StringLength))
		{
			if (pBytesWritten)
			{
				*pBytesWritten = StringLength + sizeof(StringLength);
			}
			return true;
		}
	}
	return(false);
}

bool cFile::read(cString& string)
{
	bool result = false;
	char* name =0;
	int32 StringLength=0;

	if(read(StringLength))
	{
		if (StringLength)
		{
			name = new char[StringLength];
			if(read(name, StringLength))
			{
				string = name;
				delete [](name);
				result = true;
			}
		}
		else
		{
			string.erase();
			result = true;
		}
	}
	return(result);
}

//: ACCESSORS ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

uint32 cFile::size()
{
	debug_assert(m_fileHandle, "There is no file handle open");

	uint32 CurrentPos = position();

	fseek((FILE*)m_fileHandle, (long)0, SEEK_END);

	uint32 size=position();

	setPosition(CurrentPos);

	return (size);
}

//***************************************************************
// end of file      ( cFile.cpp )

//----------------------------------------------------------
//$Log: $