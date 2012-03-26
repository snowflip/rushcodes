@echo off

 SETLOCAL ENABLEDELAYEDEXPANSION
 
for /f "tokens=1,2 " %%i in (dirList.txt) do (
					wmic environment where "name='%%i'" delete 
	  				wmic environment create name="%%i",username="<system>",VariableValue="%%j"
	  				)
	  				
rem | wmic environment where "name='%%i'" delete 
rem | wmic environment create name="%%i",username="<system>",VariableValue="%%j"

 rem echo %localFolder%



rem WMIC ENVIRONMENT WHERE "name='%localFolder%'" delete 


pause