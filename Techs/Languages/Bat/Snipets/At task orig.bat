@echo off

if exist TIME.TM (
rem echo exist
for /f "delims=" %%i in (TIME.TM) do (
set TimeToRun=%%i
)
) else (
echo not exist
echo 请输入每天几点运行该自动编译系统[H:M/9:00]:
set /p TimeToRun=
rem 后面不好跟东西了
)

echo %TimeToRun%
echo %TimeToRun%>TIME.TM

rem 每天运行该程序
AT %TimeToRun% /EVERY:M,T,W,TH,F,S,SU "%~f0"

pause