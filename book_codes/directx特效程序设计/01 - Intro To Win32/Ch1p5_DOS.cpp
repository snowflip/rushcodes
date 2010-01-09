/*
#############################################################################

  Ch1p5_DOS.cpp : Defines the entry point for the console application.

  Release:

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////
#include <conio.h>
#include <stdio.h>

/****************************************************************************

 main: Program starts here.

 ****************************************************************************/
int main(int argc, char* argv[])
{
	printf("Press any key to exit!\n");
  while (!kbhit()) { }
	return 0;
}

