echo off
echo %time% >> time.tmp
for /f "delims=: tokens=1,2 " %%i in (time.tmp) do set timePostfix=%%i_%%j
del time.tmp
echo %timePostfix%

pause