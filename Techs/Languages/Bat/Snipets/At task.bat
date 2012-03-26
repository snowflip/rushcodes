@rem 接受参数，以及TIME.TM文件，将参数在TIME.TM中定义的时间上运行
@echo off

SETLOCAL ENABLEDELAYEDEXPANSION

if exist TIME.TM (
rem echo exist
for /f "delims=" %%i in (TIME.TM) do (
echo %%i
rem AT %TimeToRun% /EVERY:M,T,W,TH,F,S,SU "%~f0"
AT %%i /interactive /EVERY:M,T,W,TH,F,S,SU "%1"
)
) else (
echo not exist
echo 请在TIME.TM文件中输入每天定点编译的时间
exit /b -1
)

pause