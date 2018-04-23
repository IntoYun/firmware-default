@echo off
::下载工具
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------Ant固件升级--------
echo     (请确保连接ST-LINK)

pause

echo 1.下载Bootloader ...
"%ST_TOOL%" write boot-v1.bin 0x8000000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
goto update_end
)

echo 2.下载应用程序 ...
"%ST_TOOL%" --reset write firmware.bin 0x08006000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
)

:update_end

if errorlevel 0 (
    echo -------下载成功------
) else (
    echo -------下载失败------
)

pause

