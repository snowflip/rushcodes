@echo off
for /f "delims=" %%i in ('cd') do echo %%~ni
rem for   %%i in ('cd') do echo %%i


pause "hello"