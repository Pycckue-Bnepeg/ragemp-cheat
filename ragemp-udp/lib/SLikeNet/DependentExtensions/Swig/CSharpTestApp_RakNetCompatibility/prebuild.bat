@echo off

REM Copyright (c) 2018, SLikeSoft UG (haftungsbeschränkt)
REM
REM This file is licensed under the MIT-style license found in the license.txt
REM file in the root directory of this source tree.

REM import mandatory parameters
set configuration=%~1
set platform=%2


REM set required variables
set targetDir=bin\x64\%configuration%

REM convert platform parameter
if "%platform%"=="x86" (
	set platform=Win32
	set targetDir=bin\x86\%configuration%
)

copy "..\..\..\Lib\RakNet_DLL_%configuration%_%platform%.dll" "%targetDir%\RakNet.dll"
exit %ERRORLEVEL%