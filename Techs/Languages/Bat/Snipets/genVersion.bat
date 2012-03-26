@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

rem 版本号1默认1，这是大版本号
rem 版本号2，3，4 月/日/时
set vn1=1
set vn2=
set vn3=
set vn4=

rem 获取正确时间版本-----------------------------------
echo %time% > time.tmp
for /f "delims=: tokens=1 " %%i in (time.tmp) do (
set vn4=%%i
)
rem 去掉小时前有可能存在的空格
for /f "tokens=1 delims= " %%i in ("%vn4%") do set vn4=%%i
echo %vn4%
del time.tmp

echo %date% > date.tmp
for /f "tokens=1,2,3 delims=-	 " %%i in (date.tmp) do (
set vn2=%%j
set vn3=%%k
)
del date.tmp
rem ------------------------------------------------

set VERSION_STR=%vn1%.%vn2%.%vn3%.%vn4%
echo %VERSION_STR%

rem 默认没有verdesc
start /WAIT GenVersion.exe VERSION.DAT %VERSION_STR%

echo "命令成功完成" 
pause "命令成功完成" 