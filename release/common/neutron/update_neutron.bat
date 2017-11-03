@echo off
::下载工具
set ESP8266_TOOL=tools/esp8266/win/esptool.exe
set ST_TOOL=tools/stlink/win/st-flash.exe
::下载参数
set UPLOAD_SPEED=460800
set UPLOAD_PORT=COM1

:start

echo -------neutron软件升级--------
echo 请选择升级内容(回车默认选择1):
echo 1. 升级stm32    (请确保连接ST-LINK)
echo 2. 升级esp8266  (请确保neutron进入esp8266升级模式)
@set select_type=1
set /p select_type=
if %select_type%==2 (
goto esp8266_update
)
goto stm32_update_start

:esp8266_update

set usart_port=%UPLOAD_PORT%
set /p usart_port=请输入串口(回车默认输入COM1):
set UPLOAD_PORT=%usart_port%

echo 请选择串口波特率(回车默认选择3.460800):
echo 1. 115200  2. 230400  3. 460800  4. 921600
set usart_speed=3
set /p usart_speed=
if %usart_speed%==1 (
set UPLOAD_SPEED=115200
goto esp8266_update_start
)
if %usart_speed%==2 (
set UPLOAD_SPEED=230400
goto esp8266_update_start
)
if %usart_speed%==3 (
set UPLOAD_SPEED=460800
goto esp8266_update_start
)
if %usart_speed%==4 (
set UPLOAD_SPEED=921600
goto esp8266_update_start
)

:esp8266_update_start
echo esp8266 升级中...
"%ESP8266_TOOL%" -v -cd nodemcu -cp %UPLOAD_PORT% -cb %UPLOAD_SPEED% -ca 0x00000 -cf neutron-net-boot.bin -ca 0x08000 -cf neutron-net.bin -ca 0xBC000 -cf default-neutron.bin -ca 0xFC000 -cf blank.bin -ca 0x100000 -cf neutron-boot.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin

goto update_end

:stm32_update_start

echo stm32   升级中...
echo %ST_TOOL%
"%ST_TOOL%" write neutron-boot.bin 0x8000000

:: 延时1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-neutron.bin 0x08020000

:update_end

if errorlevel 1 (
    echo -------升级失败------
) else (
    echo -------升级成功------
)

pause

