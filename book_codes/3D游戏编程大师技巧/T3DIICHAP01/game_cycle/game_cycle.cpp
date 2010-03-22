#include <iostream>
using namespace std;

//游戏状态
enum E_GAME_STATE
{
	GAME_INIT,
	GAME_MENU,
	GAME_START,
	GAME_RUN,
	GAME_RESTART,
	GAME_EXIT
};

//全局变量
E_GAME_STATE g_gameState = GAME_INIT;
int error = 0;


//主游戏逻辑
#define FUNC(x)		\
	bool (x)()		\
{return true;}  \

FUNC ( Init )					//游戏初始函数
E_GAME_STATE  Menu() {return GAME_START;} ;		//显示菜单
FUNC ( SetupForRun )			//运行前的准备
FUNC ( Clear )					//清屏
FUNC ( GetInput )				//获取输入
FUNC ( DoLogic )				//执行逻辑和AI				
FUNC ( RenderFrame )			//渲染下一帧
FUNC ( Wait )					//显示速度同步
FUNC ( FixUp )					
FUNC ( ReleaseAndCleanup )


//主循环
int main()
{
	while(g_gameState != GAME_EXIT)
	{
		//检查游戏循环状态
		switch(g_gameState)
		{
		case GAME_INIT:
			{
				//分配所有内存和资源
				Init();

				g_gameState = GAME_MENU;		//切换到菜单模式
			}
			break;

		case GAME_MENU:
			{
				//调用主菜单函数，让它来切换状态
				g_gameState = Menu();			//这里可以直接切换到运行状态
			}
			break;

		case GAME_START:	//游戏要运行了
			{
				//这种状态是可选的，通常用于为游戏运行做准备
				//还可做一些清理工作
				SetupForRun();

				g_gameState = GAME_RUN;			//切换到运行
			}
			break;

		case GAME_RUN:
			{
				//这部分包含整个游戏的逻辑循环
				//改变游戏状态的唯一方式是通过用户交互

				//清屏
				Clear();

				GetInput();			//读取输入

				DoLogic();			//执行游戏逻辑和AI

				RenderFrame();		//绘制下一帧

				Wait();				//将游戏同步到30帧/s
			}
			break;

		case GAME_RESTART:
			{
				//清理状态
				//用于在重新运行游戏之前解决遗留问题
				FixUp();

				//切换到菜单状态
				g_gameState = GAME_MENU;
			}
			break;

		case GAME_EXIT:
			{
				//如果游戏处于这种状态，则释放所有占用的资源
				ReleaseAndCleanup();

				error = 0;

				//不用在切换状态，因为处于退出状态后，在下一次循环中，代码将推出循环，返回到操作系统
			}
			break;

		default:
			break;
		}//end switch
	}//end while

	return error;		//将错误码返回给操作系统
}