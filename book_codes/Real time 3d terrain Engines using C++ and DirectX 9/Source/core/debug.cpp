#define CDEBUG_CPP
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
#include "debug.h"
#include "debug_channel.h"
#include "stack_trace.h"
#include <windows.h>
#include <dxerr9.h>

using namespace gaia;

static cDefaultOutputChannel g_defaultChannel;
cDebugMessageHandler gaia::g_Debug;


#ifdef _DEBUG
#define _STACKTRACE
#endif



/*	cDebugMessageHandler
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
cDebugMessageHandler::cDebugMessageHandler()
:m_openChannels(0)
{
	memset(m_channel, 0, sizeof(m_channel));
	m_channel[0] = &g_defaultChannel;
	openChannel(0);
}


/*	~cDebugMessageHandler
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
cDebugMessageHandler::~cDebugMessageHandler()
{
	// close the default system channel
	closeChannel(0);

	// panic if channels have been left open.
	debug_assert(m_openChannels.isEmpty(), "debug channels left open");
}


void cDebugMessageHandler::setChannelMessageFilter(int index, u32Flags flags)
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	debug_assert(m_channel[index], "invalid channel index");

	m_channel[index]->messageFilter = flags;
}

void cDebugMessageHandler::addChannelMessageFilter(int index, u32Flags flags)
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	debug_assert(m_channel[index], "invalid channel index");

	m_channel[index]->messageFilter.setFlags(flags);
}

void cDebugMessageHandler::removeChannelMessageFilter(int index, u32Flags flags)
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	debug_assert(m_channel[index], "invalid channel index");

	m_channel[index]->messageFilter.clearFlags(flags);
}

bool cDebugMessageHandler::setChannel(int index, cDebugMessageChannel* pChannel)
{
	// channel 0 is reserved. index must be in range 1-nMaxChannels
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	
	if (!m_channel[index])
	{
		m_channel[index] = pChannel;
		return true;
	}
	// the channel is already in use
	return false;
}

void cDebugMessageHandler::clearChannel(int index)
{
	// channel 0 is reserved. index must be in range 1-nMaxChannels
	debug_assert(index > 0 && index<nMaxChannels, "invalid channel index");
	debug_assert(!TEST_BIT(m_openChannels,index), "the channel is currently open");

	m_channel[index] = 0;
	m_openChannels.clearBit(index);
}

bool cDebugMessageHandler::openChannel(int index)
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");

	if (!TEST_BIT(m_openChannels,index))
	{
		if (m_channel[index] && m_channel[index]->open())
		{
			m_openChannels.setBit(index);
			return true;
		}

		// either there is no channel, or it failed to open
		return false;
	}

	// the channel is already open
	return true;
}

void cDebugMessageHandler::closeChannel(int index)
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");

	if (TEST_BIT(m_openChannels,index))
	{
		if (m_channel[index])
		{
			m_channel[index]->close();
			m_openChannels.clearBit(index);
		}
	}
}


u32Flags cDebugMessageHandler::channelFilter(int index)const
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	debug_assert(m_channel[index], "invalid channel index");

	return m_channel[index]->messageFilter;
}


// route text input using the internal default message flag
void cDebugMessageHandler::output(const tchar* text, ...)
{
	// build the va_list of optional arguments
	va_list		arglist;
	va_start(arglist, text);

	// call the va_list version of output
	processMessage(k_defaultMessageFlag, text, arglist);

	// end the optional argument list use
	va_end(arglist);
}

// route text input using the message flag provided
void cDebugMessageHandler::output(uint32 messageFlags, const tchar* text, ...)
{
	// build the va_list of optional arguments
	va_list		arglist;
	va_start(arglist, text);

	// call the va_list version of output
	processMessage(messageFlags, text, arglist);

	// end the optional argument list use
	va_end(arglist);
}

// the function which handles the actual routing of text messages
void cDebugMessageHandler::processMessage(uint32 messageFlags, const tchar* text, va_list arglist)
{
	// are any channels currently open?
	if (m_openChannels)
	{
		// build our output string
		tchar	buffer[nMaxOutputStringSize+1];
		int Size = _vsntprintf(buffer, nMaxOutputStringSize, text, arglist);

		// if a string was built...
		if(Size > 0)
		{
			// run through all the channels
			for (int i=0; i<nMaxChannels; ++i)
			{
				// if the channel is open...
				if (TEST_BIT(m_openChannels,i))
				{
					// and it accepts this message type...
					if (m_channel[i]->messageFilter.testAny(messageFlags))
					{
						// then send the message through
						m_channel[i]->write(buffer);
					}
				}
			}
		}
	}
}


gaia::ERROR_RESULT gaia::notifyError(uint32 errorCode, const tchar* fileName, int lineNumber)
{
	// if no error code is provided, get the last known error
	if (errorCode == 0)
	{
		errorCode = GetLastError();
	}

	// use DirectX to supply a string and description for our error.
	// This will handle all known DirectX error codes (HRESULTs)
	// as well as Win32 error codes normally found via FormatMessage
	const tchar* pErrorString = DXGetErrorString9(errorCode);
 	const tchar* pErrorDescription = DXGetErrorDescription9(errorCode);

	// pass the data on to the message box
	ERROR_RESULT result = displayError(	_text("Debug Error!"),
										pErrorString,
										pErrorDescription,
										fileName,
										lineNumber);

	// Put the incoming last error back.
	SetLastError(errorCode);

	return(result);
}

gaia::ERROR_RESULT gaia::notifyAssertion(const tchar* condition, const tchar* description, const tchar* fileName, int lineNumber)
{
	// pass the data on to the message box
	ERROR_RESULT result = displayError(_text("Assert Failed!"),
										condition,
										description,
										fileName,
										lineNumber);

	return(result);

}

gaia::ERROR_RESULT gaia::displayError(const tchar* errorTitle,
										  const tchar* errorText,
										  const tchar* errorDescription,
										  const tchar* fileName, 
										  int lineNumber)
{
	const	int		MODULE_NAME_SIZE = 255;
	tchar moduleName[MODULE_NAME_SIZE];

	// attempt to get the module name
    if (!GetModuleFileName(NULL, moduleName, MODULE_NAME_SIZE))
    {
        _tcscpy(moduleName, _text("<unknown application>"));
    }

	// if stack tracing is enabled, build a string containing the 
	// unwound stack information
#ifdef _STACKTRACE
		const	int		STACK_STRING_SIZE = 255;
		tchar stackText[STACK_STRING_SIZE];

		buildStackTrace(stackText, STACK_STRING_SIZE, 2);
#else
		tchar stackText[] = _text("<stack trace disabled>");
#endif

	// build a collosal string containing the entire asster message
	const	int		MAX_BUFFER_SIZE = 1024;
	tchar	buffer[MAX_BUFFER_SIZE];

	int Size = _sntprintf(	buffer, 
							MAX_BUFFER_SIZE, 
							_text(	"%s\n\n"						\
									 "Program : %s\n"               \
									 "File : %s\n"                  \
									 "Line : %d\n"                  \
									 "Error: %s\n"			\
									 "Comment: %s\n"						\
									 "\nStack:\n%s\n\n"						\
									 "Abort to exit (or debug), Retry to continue,\n"\
									 "Ignore to disregard all occurances of this error\n"),
							errorTitle,
							moduleName,
							fileName,
							lineNumber,
							errorText,
							errorDescription,
							stackText
							);


	// place a copy of the message into the clipboard
	if (OpenClipboard(NULL))
	{
		uint32 bufferLength = _tcsclen(buffer);
		HGLOBAL hMem = GlobalAlloc(GHND|GMEM_DDESHARE, bufferLength+1);

		if (hMem)
		{
			uint8* pMem = (uint8*)GlobalLock(hMem);
			memcpy(pMem, buffer, bufferLength);
			GlobalUnlock(hMem);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
		}

		CloseClipboard();
	}


	// find the top most window of the current application
	HWND hWndParent = GetActiveWindow ( ) ;
	if ( NULL != hWndParent )
	{
		hWndParent = GetLastActivePopup ( hWndParent ) ;
	}

	// put up a message box with the error
	int iRet = MessageBox ( hWndParent,
							buffer,
							_text ( "ERROR NOTIFICATION..." ),
							MB_TASKMODAL
							|MB_SETFOREGROUND
							|MB_ABORTRETRYIGNORE
							|MB_ICONERROR);

	// Figure out what to do on the return.
	if (iRet == IDRETRY)
	{
		// ignore this error and continue
		return (VR_CONTINUE);
	}
	if (iRet == IDIGNORE)
	{
		// ignore this error and continue,
		// plus never stop on this error again (handled by the caller)
		return (VR_IGNORE);
	}

	// The return has to be IDABORT, but does the user want to enter the debugger
	// or just exit the application?
	iRet = MessageBox ( hWndParent,
						"Do you wish to debug the last error?",
						_text ( "DEBUG OR EXIT?" ),
						MB_TASKMODAL
						|MB_SETFOREGROUND
						|MB_YESNO
						|MB_ICONQUESTION);

	if (iRet == IDYES)
	{
		// inform the caller to break on the current line of execution
		return (VR_BREAKPOINT);
	}

	// must be a full-on termination of the app
	ExitProcess ( (UINT)-1 ) ;
	return (VR_ABORT);
}



//***************************************************************
// end of file      ( Debug.cpp )

//----------------------------------------------------------
//$Log: $