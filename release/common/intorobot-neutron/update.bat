@echo off
::下载工具
set ESP8266_TOOL=tools/esp8266/win/esptool.exe
set ST_TOOL=tools/stlink/win/st-flash.exe
set DFU_TOOL=tools/dfu-util/win/dfu-util.exe
set DFUSUFFIX=tools/dfu-util/win/dfu-suffix.exe
::下载参数
set UPLOAD_SPEED=460800
set UPLOAD_PORT=COM1

:start

echo -------IntoRobot Neutron软件升级--------
echo 请选择升级内容(回车默认选择1):
echo 1. 应用程序       (请手动进入DFU模式)
echo 2. 完整包         (请确保连接ST-LINK)
echo 3. 升级esp8266    (请确保neutron进入esp8266升级模式)

set select_type=1
set /p select_type=
if %select_type%==2 (
goto all_update_start
)
if %select_type%==3 (
goto esp8266_update_start
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

echo 1.下载Bootloader ...
"%ST_TOOL%" write boot-v3.bin 0x8000000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
goto update_end
)

echo 2.下载应用程序 ...
"%ST_TOOL%" --reset write firmware.bin 0x08020000
if %errorlevel%==0 (
echo 成功
) else (
echo 失败
)
goto update_end

:esp8266_update_start

set usart_port=%UPLOAD_PORT%
set /p usart_port=请输入串口(回车默认输入COM1):
set UPLOAD_PORT=%usart_port%

echo 请选择串口波特率(回车默认选择3.460800):
echo 1. 115200  2. 230400  3. 460800  4. 921600
set usart_speed=3
set /p usart_speed=
if %usart_speed%==1 (
set UPLOAD_SPEED=115200
)
if %usart_speed%==2 (
set UPLOAD_SPEED=230400
)
if %usart_speed%==3 (
set UPLOAD_SPEED=460800
)
if %usart_speed%==4 (
set UPLOAD_SPEED=921600
)

echo esp8266 升级中...
"%ESP8266_TOOL%" -v -cd nodemcu -cp %UPLOAD_PORT% -cb %UPLOAD_SPEED% -ca 0x00000 -cf net-boot.bin -ca 0x08000 -cf net.bin -ca 0xBC000 -cf firmware.bin -ca 0xFC000 -cf blank.bin -ca 0x100000 -cf boot-v3.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin
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

