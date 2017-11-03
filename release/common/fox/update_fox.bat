@echo off
::下载工具
set ST_TOOL=tools/stlink/win/st-flash.exe
set DFU_TOOL=tools/dfu-util/win/dfu-util.exe


:start

echo -------Fox固件升级--------
echo 请选择升级内容(回车默认选择1):
echo 1. 升级stm32    (请确保连接ST-LINK)
echo 2. 升级SPI Flash  (请确保电脑已安装DFU驱动，并且Fox已进入DFU升级模式)

@set select_type=1
set /p select_type=
if %select_type%==2 (
goto spi_flash_update_start
)
goto stm32_update_start

:spi_flash_update_start
echo spi flash 升级中...
echo %DFU_TOOL%
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D default-fox.dfu

goto update_end

:stm32_update_start

echo stm32   升级中...
echo %ST_TOOL%
"%ST_TOOL%" write fox-boot.bin 0x8000000

:: 延时1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-fox.bin 0x08020000

:update_end

if errorlevel 1 (
    echo -------升级失败------
) else (
    echo -------升级成功------
)

pause

