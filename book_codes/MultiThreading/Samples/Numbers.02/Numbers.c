// 线程内核对象的引用计数问题
// 也是内核对象的一个普遍性质，要特别注意

/*
 * Numbers.c
 *
 * Sample code for "Multithreading Applications in Win32"
 * This is from Chapter 2, Listing 2-1
 *
 * Starts five threads and gives visible feedback
 * of these threads running by printing a number
 * passed in from the primary thread.
 *
 */

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI ThreadFunc(LPVOID);

int main()
{
    HANDLE hThrd;
    DWORD threadId;
    int i;
    
    for (i=0; i<5; i++)
    {
        hThrd = CreateThread(NULL,
            0,
            ThreadFunc,
            (LPVOID)i,
            0,
            &threadId );
        if (hThrd)
        {
            printf("Thread launched %d\n", i);
            CloseHandle(hThrd);							// 理解这一点很重要，线程内核对象的初始引用计数就为2，
														//只有当已经启动的线程也同时结束（从而引用计数减一），该线程内核对象才会被释放。
        }
    }
    // Wait for the threads to complete.
    // We'll see a better way of doing this later.
    Sleep(2000);

    return EXIT_SUCCESS;
}

DWORD WINAPI ThreadFunc(LPVOID n)
{
    int i;
    for (i=0;i<10;i++)
	    printf("%d%d%d%d%d%d%d%d\n",n,n,n,n,n,n,n,n);
    return 0;
}
