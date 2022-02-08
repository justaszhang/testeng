@echo OFF
REM kopiert alle wichtigen Dateien nach 
REM c:\Programme\%dirName%
REM oder
REM c:\Program Files\%dirName%
setlocal ENABLEEXTENSIONS DISABLEDELAYEDEXPANSION



REM den dir_name erzeugen
for /d %%I in (.) do set dirName=%%~nI

echo %dirName%

REM alle .dll nach windows\system32
REM
REM for %%i in ("*.dll") do (
REM     if not exist "%SystemRoot%\system32\%%i" (
REM         echo copy "%%i" "%SystemRoot%\system32"
REM         copy /Y "%%i" "%SystemRoot%\system32"
REM     )    
REM )
REM 

REM
for %%i in ("*.dll") do (
    echo copy "%%i" "%SystemRoot%\system32"
    copy /-Y "%%i" "%SystemRoot%\system32"
)


:ende
endlocal
pause





