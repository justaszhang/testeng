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
for %%i in ("*.dll") do (
    echo copy "%%i" "..\Projekte\RFP_IP_pro68\FUNCTION\FP_35\%%i"
    copy /Y "%%i" "..\Projekte\RFP_IP_pro68\FUNCTION\FP_35\%%i"
    echo copy "%%i" "..\Projekte\RFP_IP_pro68\FINAL\RFP_35\%%i"
    copy /Y "%%i" "..\Projekte\RFP_IP_pro68\FINAL\RFP_35\%%i"
    echo copy "%%i" "..\Rte5_3_en\%%i"
    copy /Y "%%i" "..\Rte5_3_en\%%i"
)


:ende
endlocal
pause





