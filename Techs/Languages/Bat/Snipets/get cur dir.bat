rem @echo off
for /f "delims=" %%i in (%cd%) do set CurDir=%%i

echo %CurDir%

pause