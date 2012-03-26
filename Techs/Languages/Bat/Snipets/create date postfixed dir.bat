echo off
echo %date% >> date.tmp
for /f "tokens=1,2,3 delims=-	 " %%i in (date.tmp) do set datePostfix=%%i_%%j_%%k
del date.tmp
echo %datePostfix%

pause