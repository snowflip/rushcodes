@echo off
set /p vn3=输入版本号1:
set /p vn2=输入版本号2:
set /p vn1=输入版本号3:
set /p vn0=输入版本号4:
rem set /p vndesc=输入版本描述:

rem bat目录
set BatDir=%cd%

rem 一些预设目录结构，将不会修改
set version_str=%vn3%.%vn2%.%vn1%.%vn0%
set version_folder=%BatDir%\%vn3%_%vn2%_%vn1%_%vn0%
set VersionMakeDir=%BatDir%\..\BinTools\VersionMaker\release
set VersionInfoExe=%VersionMakeDir%\VersionInfo.exe
set MaterialPreParserExe=%VersionMakeDir%\MaterialPreParser.exe
set CrossFileMakerExe=%VersionMakeDir%\CrossFileMakerExe1.exe
set GameExeDir=%BatDir%\..\client\Bin\%VersionType%

echo %version_str%
echo %version_folder%
echo %VersionMakeDir%
echo %VersionInfoExe%
echo %MaterialPreParserExe%
echo %CrossFileMakerExe%
echo %GameExeDir%


pause