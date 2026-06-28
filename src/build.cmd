@echo off
setlocal

cd /d "%~dp0.."

for /f "delims=" %%I in ('where cl.exe 2^>nul') do (
    set "CL_EXE=%%I"
    goto :found_cl
)
:found_cl
if "%CL_EXE%"=="" (
    where cl >nul 2>nul
)
if "%CL_EXE%"=="" (
    set "CL_EXE=cl"
)
if "%CL_EXE%"=="cl" (
    where cl >nul 2>nul
    if errorlevel 1 (
        echo Run this from an x64 Visual Studio developer command prompt.
        exit /b 1
    )
)
if not "%CL_EXE%"=="cl" if not exist "%CL_EXE%" (
    echo Run this from an x64 Visual Studio developer command prompt.
    exit /b 1
)

set "OUT=Work\build"
mkdir "%OUT%" 2>nul

set "EFI_CL=/nologo /c /O1 /Oi /GS- /GR- /EHs-c- /Zl /W4"
set "EFI_LD=/nologo /dll /nodefaultlib /machine:x64 /subsystem:EFI_BOOT_SERVICE_DRIVER /fixed:no /dynamicbase:no /nxcompat:no"

"%CL_EXE%" %EFI_CL% /Fo:%OUT%\Dxe.obj src\Dxe.c || exit /b 1
link %EFI_LD% /entry:DxeEntry /out:%OUT%\Dxe.efi %OUT%\Dxe.obj || exit /b 1

"%CL_EXE%" %EFI_CL% /Fo:%OUT%\Smm.obj src\Smm.c || exit /b 1
link %EFI_LD% /entry:SmmEntry /out:%OUT%\Smm.efi %OUT%\Smm.obj || exit /b 1

"%CL_EXE%" %EFI_CL% /Fo:%OUT%\Payload.obj src\Payload.c || exit /b 1
link %EFI_LD% /entry:PayloadEntry /out:%OUT%\Payload.efi %OUT%\Payload.obj || exit /b 1

"%CL_EXE%" /nologo /W4 /O2 /DUNICODE /D_UNICODE /Fo:%OUT%\Client.obj /Fe:%OUT%\Client.exe src\Client.c Advapi32.lib || exit /b 1

echo Built:
echo   %OUT%\Dxe.efi
echo   %OUT%\Smm.efi
echo   %OUT%\Payload.efi
echo   %OUT%\Client.exe
