@echo off
call vendor\bin\premake\premake5.exe vs2019
if %ERRORLEVEL% NEQ 0 (
	PAUSE
)
