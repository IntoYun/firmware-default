@echo off
::下载工具
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------Ant固件升级--------
echo 升级stm32    (请确保连接ST-LINK)

echo stm32   升级中...
echo %ST_TOOL%
"%ST_TOOL%" write ant-boot.bin 0x8000000

:: 延时1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-ant.bin 0x08006000

:update_end

if errorlevel 1 (
    echo -------升级失败------
) else (
    echo -------升级成功------
)

pause

