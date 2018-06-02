@echo off
::下载工具
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------IntoRobot Atom固件升级--------
echo 请选择下载内容(回车默认选择1):
echo 1. 完整包    (请确保连接ST-LINK)

pause

echo 1.下载应用程序 ...
"%ST_TOOL%" --reset write firmware.bin 0x08000000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
)

if errorlevel 0 (
    echo -------下载成功------
) else (
    echo -------下载失败------
)

pause

