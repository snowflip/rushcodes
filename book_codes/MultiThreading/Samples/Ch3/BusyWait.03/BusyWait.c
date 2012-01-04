//  该事例在真正多核的机器上也许是无法说明的

// 在我的i7-950的机子上的结果，显然和作者的预期有背
// 但是多次测试结果是有异的，有时和作者的预期相同，所以，多线程的执行顺序是完全无法估计的
//Timing normal function call...
//PI = 3.141
//Function call took: 0.062 seconds
//
//Timing thread + busy loop...
//PI = 3.141
//Thread + busy loop took: 0.047 seconds

/*
 * BusyWait.c
 *
 * Sample code for Multithreading Applications in Win32
 * This is from Chapter 3, Listing 3-1
 *
 * Demonstrate the effect on performance
 * of using a busy loop. First call the
 * worker routine with just a function call
 * to get a baseline performance reading,
 * then create a second thread and a
 * busy loop.
 *
 * Build command: cl /MD busywait.c
 */

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "MtVerify.h"

DWORD WINAPI ThreadFunc(LPVOID);

int main()
{
    HANDLE hThrd;
    DWORD exitCode = 0;
    DWORD threadId;
    DWORD begin;
    DWORD elapsed;

    puts("Timing normal function call...");
    begin = GetTickCount();
    ThreadFunc(0);
    elapsed = GetTickCount()-begin;
    printf("Function call took: %d.%.03d seconds\n\n", elapsed/1000, elapsed%1000);
    
    puts("Timing thread + busy loop...");
    begin = GetTickCount();

    MTVERIFY( hThrd = CreateThread(NULL,
        0,
        ThreadFunc,
        (LPVOID)1,
        0,
        &threadId )
    );

    /* This busy loop chews up lots of CPU time */
    for (;;)
    {
        GetExitCodeThread(hThrd, &exitCode);
        if ( exitCode != STILL_ACTIVE )
            break;
    }

    elapsed = GetTickCount()-begin;
    printf("Thread + busy loop took: %d.%.03d seconds\n",
                elapsed/1000, elapsed%1000);

    MTVERIFY( CloseHandle(hThrd) );

    return EXIT_SUCCESS;
}


/*
 * Cute little busy work routine that computes the value
 * of PI using probability.  Highly dependent on having
 * a good random number generator (rand is iffy)
 */
DWORD WINAPI ThreadFunc(LPVOID n)
{
    int i;
    int inside = 0;
    double val;

    UNREFERENCED_PARAMETER(n);

    /* Seed the random-number generator */
    srand( (unsigned)time( NULL ) );

    for (i=0; i<1000000; i++)
    {
        double x = (double)(rand())/RAND_MAX;
        double y = (double)(rand())/RAND_MAX;
        if ( (x*x + y*y) <= 1.0 )
            inside++;
    }
    val = (double)inside / i;
    printf("PI = %.4g\n", val*4);
    return 0;
}
