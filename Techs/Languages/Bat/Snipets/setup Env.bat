@echo off
Rem 从svn uplist.txt中解析出需要更新的文件夹目录

 SETLOCAL ENABLEDELAYEDEXPANSION
 
 rem 获取当前dir
for /f  %%i in ('cd') do set localFolder=%%i
rem echo %localFolder%

for /f "tokens=1,2 " %%i in (dirList.txt) do (
					wmic environment where "name='%%i'" delete 
	  				wmic environment create name="%%i",username="<system>",VariableValue="%localFolder%\%%j"
	  				)

echo 命令成功完成
pause 