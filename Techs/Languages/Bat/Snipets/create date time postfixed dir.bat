echo off
echo %time% >> time.tmp
for /f "delims=: tokens=1,2 " %%i in (time.tmp) do set timePostfix=%%i_%%j
del time.tmp
rem echo %timePostfix%

echo %date% >> date.tmp
for /f "tokens=1,2,3 delims=-	 " %%i in (date.tmp) do set datePostfix=%%i_%%j_%%k
del date.tmp
rem echo %datePostfix%

echo %datePostfix%%timePostfix%

mkdir "%datePostfix%%timePostfix%"

pause