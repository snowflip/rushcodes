rem 自动版本编译系统
@echo off
rem %comspec% /k ""C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"" x86

rem 首先更新svn
start /WAIT cmd /c "svn update.bat"

SETLOCAL ENABLEDELAYEDEXPANSION

rem 每天自动运行的时间------------------------
if exist TIME.TM (
rem echo exist
for /f "delims=" %%i in (TIME.TM) do set TimeToRun=%%i
) else (
echo not exist
echo 请输入每天几点运行该自动编译系统[H:M/9:00]:
set /p TimeToRun=
rem 后面不好跟东西了
)

if exist TIME.TM (
rem echo exist
) else (
rem echo %TimeToRun%
echo %TimeToRun%>TIME.TM

rem 每天运行该程序
AT %TimeToRun% /EVERY:M,T,W,TH,F,S,SU "%~f0"
if %errorlevel% equ 0 (
rem echo 自动运行添加成功
) else (
echo 自动运行添加失败
pause 
exit
)
)


rem ------------------------------------------------

rem 获取目录-----------------------------------
set VERSIONDIR=
if exist VERSIONDIR.VER	(
rem echo exist
for /f "delims=" %%i in (VERSIONDIR.VER) do set VERSIONDIR=%%i
) else (
echo not exist
echo 请输入版本文件夹位置,请使用绝对值:
set /p VERSIONDIR=
rem 后面不好跟东西了
)
rem echo %VERSIONDIR%

rem 确保格式正确
rem echo %VERSIONDIR:~-1,1%
if "%VERSIONDIR:~-1,1%" == "\" (
rem echo good format
) else (
set VERSIONDIR=%VERSIONDIR%\

)

rem echo %VERSIONDIR%

rem 确保文件夹有效
mkdir "%VERSIONDIR%test"
if %errorlevel% equ 0 (
rem echo 文件夹有效
rmdir "%VERSIONDIR%test"
) else (
echo 您输入的文件夹无效！
pause 
exit
)

echo %VERSIONDIR%>VERSIONDIR.VER
rem ------------------------------------------------

rem 编译-------------------------------------------
echo 建立日期 %date% %time%
echo 建立日期 %date% %time%>build.log
del output.log
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" ClientMain.sln /project ClientMain /Rebuild Release /out output.log
type output.log>>build.log
if %errorlevel% equ 0 (
rem echo 编译成功
) else (
echo 编译失败
pause 
exit
)
rem ----------------------------------------------

rem 获取正确时间-----------------------------------
echo %time% > time.tmp
for /f "delims=: tokens=1,2 " %%i in (time.tmp) do set timePostfix=%%i_%%j
del time.tmp
rem echo %timePostfix%

echo %date% > date.tmp
for /f "tokens=1,2,3 delims=-	 " %%i in (date.tmp) do set datePostfix=%%i_%%j_%%k
del date.tmp
rem echo %datePostfix%
rem ------------------------------------------------


rem 创建版本文件夹---------------------------------
echo %datePostfix% %timePostfix%
set DESTDIR="%VERSIONDIR%%datePostfix% %timePostfix%"
echo %DESTDIR%
mkdir %DESTDIR%
rem ------------------------------------------------


rem 拷贝到版本文件夹------------------------------
copy /Y "..\bin\release\*.*" %DESTDIR%
rem ------------------------------------------------

echo 完成构建
echo 完成构建>>build.log
copy build.log %DESTDIR%
pause 