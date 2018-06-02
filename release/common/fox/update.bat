@echo off
::下载工具
set ST_TOOL=tools/stlink/win/st-flash.exe
set DFU_TOOL=tools/dfu-util/win/dfu-util.exe
set DFUSUFFIX=tools/dfu-util/win/dfu-suffix.exe

:start

echo -------IntoRobot Fox固件升级--------
echo 请选择升级内容(回车默认选择1):
echo 1. 应用程序       (请手动进入DFU模式)
echo 2. 完整包         (请确保连接ST-LINK)
echo 3. 升级SPI Flash  (请手动进入DFU模式)

set select_type=1
set /p select_type=
if %select_type%==2 (
goto all_update_start
)
if %select_type%==3 (
goto spi_flash_update_start
)
goto app_update_start

:app_update_start

copy firmware.bin firmware.dfu >nul 2>nul
"%DFUSUFFIX%" -v 0483 -p df11 -a firmware.dfu >nul 2>nul
set /p=下载应用程序   ... <nul
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x08020000:leave -D firmware.dfu
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
)
goto update_end

:all_update_start

set /p=下载Bootloader ... <nul
"%ST_TOOL%" write boot-v1.bin 0x8000000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
goto update_end
)

set /p=下载应用程序   ... <nul
:: 延时1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write firmware.bin 0x08020000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
)
goto update_end

:spi_flash_update_start

copy firmware.bin firmware.dfu >nul 2>nul
"%DFUSUFFIX%" -v 0483 -p df11 -a firmware.dfu >nul 2>nul
echo spi flash 升级中...
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D firmware.dfu
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

