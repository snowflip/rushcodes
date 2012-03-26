@echo off
Rem 从svn uplist.txt中解析出需要更新的文件夹目录
Rem 提供了两个版本，一个普通svn版，一个tortoiseSvn版，TortoiseSvn版还写的稍微高级了点，但是悲剧的是在有的机子上有莫名其妙的无法访问文件夹权限的bug，当然不是我的脚本问题，是tortoiseSvn的问题。

Rem Normal svn version
rem for /f "eol=#" %%i in (svnUplist.txt) do echo %%i & svn update %%i


Rem TortoiseSvn version 把文件夹整理完后，一次发给tortoiseProc
set localFolder=%~d0
SETLOCAL ENABLEDELAYEDEXPANSION
for /f "eol=#" %%i in (svnUplist.txt) do set allFolder=!allFolder!%localFolder%\%%i*
set allFolder=%allFolder:~0,-1%
Rem echo %allFolder%

TortoiseProc /command:update  /path:"%allFolder%"  /closeonend:1

echo "命令成功完成" 
rem pause "命令成功完成" 