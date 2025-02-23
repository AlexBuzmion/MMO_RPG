@echo on

REM Define relative paths
set source_path=%~dp0OpenSSLFiles
set target_path=%~dp0Packaged_Builds_1\WindowsServer\MMO_Game\Binaries\Win64

REM Ensure source path exists
if not exist "%source_path%\libcrypto-3-x64.dll" (
    echo ERROR: OpenSSL file libcrypto-3-x64.dll not found in "%source_path%".
    exit /b 1
)
if not exist "%source_path%\libssl-3-x64.dll" (
    echo ERROR: OpenSSL file libssl-3-x64.dll not found in "%source_path%".
    exit /b 1
)

REM Ensure target path exists
if not exist "%target_path%" (
    echo ERROR: Target path "%target_path%" does not exist.
    exit /b 1
)

REM Copy files
copy "%source_path%\libcrypto-3-x64.dll" "%target_path%"
copy "%source_path%\libssl-3-x64.dll" "%target_path%"

echo OpenSSL files copied successfully to "%target_path%".
