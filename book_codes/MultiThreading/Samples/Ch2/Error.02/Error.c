// 该程序演示了如何使用MTVERIFY
// 以及线程的引用计数问题
/*
 * Error.c
 *
 * Sample code for "Multithreading Applications in Win32"
 * This is from Chapter 2, Listing 2-4
 *
 * Demonstrate ExitThread
 */

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "MtVerify.h"

DWORD WINAPI ThreadFunc(LPVOID);

int main()
{
    HANDLE hThrd;
    DWORD exitCode = 0;
    DWORD threadId;
    
    MTVERIFY( hThrd = CreateThread(NULL,
        0,
        ThreadFunc,
        (LPVOID)1,
        0,
        &threadId )
    );
    if (hThrd)
        printf("Thread launched\n");

    MTVERIFY( CloseHandle(hThrd) );					// 这里程序已经宣布放弃对thread的引用了，handle就失效了

    for(;;)
    {
        BOOL rc;
        MTVERIFY( rc = GetExitCodeThread(hThrd, &exitCode) );	// 失效了。。。。

		if (rc || exitCode != STILL_ACTIVE )
				break;
    }

    printf("Thread returned %d\n", exitCode);

    return EXIT_SUCCESS;
}


DWORD WINAPI ThreadFunc(LPVOID n)
{
    printf("Thread running\n");
    return 0;
}
