@echo off
rem 获取版本文件夹，并且确保其后面跟一个\
SETLOCAL ENABLEDELAYEDEXPANSION

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
echo %VERSIONDIR%

rem 确保格式正确
rem echo %VERSIONDIR:~-1,1%
if "%VERSIONDIR:~-1,1%" == "\" (
rem echo good format
) else (
set VERSIONDIR=%VERSIONDIR%\

)

echo %VERSIONDIR%

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

pause