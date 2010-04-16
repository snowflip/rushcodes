//c++类原理

#include <iostream>
#include <Windows.h>
using namespace std;

struct Vtable;

//c++类，它由成员函数和变量组成，成员函数
typedef struct{
	Vtable*			pointer;
	DWORD			count;
	char*			buffer;
}IExample;

//vtable中的函数，都有一个应用的类的arg，其实就是this指针
typedef void SetStringPtr(IExample* exp, char *);
typedef char* GetStringPtr(IExample* exp);

//专门一个struct存储函数指针，也就是c++ class中的第一个元素--vtable
struct Vtable
{
	SetStringPtr*		setstringPtr;
	GetStringPtr*		getstringPtr;
};

void SetString(IExample* exp, char* str)
{	
	exp->buffer = str;
}
char* GetString(IExample* exp)
{
	return exp->buffer;
}

int main()
{
	Vtable* vtab = new Vtable();
	vtab->setstringPtr = SetString;
	vtab->getstringPtr = GetString;

	IExample* example;
	example = (IExample*)GlobalAlloc(GMEM_FIXED, sizeof(IExample));		

	example->pointer = vtab;

	//调用成员函数
	example->pointer->setstringPtr(example, "hello, world");
	cout << example->pointer->getstringPtr(example);
}

