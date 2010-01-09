// Include Directives ///////////////////////////////////////////////////////
#include <conio.h>
#include <stdio.h>

//kbhit用于检测是否有键按下

int main(int argc, char* argv[])
{
	printf("Press any key to exit!\n");
  while (!kbhit()) { }
	return 0;
}

