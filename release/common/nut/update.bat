@echo off
::下载工具
set ESP8266_TOOL=tools/esp8266/win/esptool.exe
::下载参数
set UPLOAD_SPEED=921600
set UPLOAD_PORT=COM1

:start

echo -------nut软件升级--------

set usart_port=%UPLOAD_PORT%
set /p usart_port=请输入串口(回车默认输入COM1):
set UPLOAD_PORT=%usart_port%

echo 请选择串口波特率(回车默认选择4.921600):
echo 1. 115200  2. 230400  3. 460800  4. 921600
set usart_speed=3
set /p usart_speed=
if %usart_speed%==1 (
set UPLOAD_SPEED=115200
goto update_start
)
if %usart_speed%==2 (
set UPLOAD_SPEED=230400
goto update_start
)
if %usart_speed%==3 (
set UPLOAD_SPEED=460800
goto update_start
)
if %usart_speed%==4 (
set UPLOAD_SPEED=921600
goto update_start
)
set UPLOAD_SPEED=460800

:update_start
echo esp8266 升级中...

"%ESP8266_TOOL%"  -v -cd nodemcu -cp %UPLOAD_PORT% -cb %UPLOAD_SPEED% -ca 0x00000 -cf boot-v3.bin -ca 0x14000 -cf firmware.bin -ca 0x114000 -cf firmware.bin -ca 0x194000 -cf boot-v3.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin 

if errorlevel 1 (
    echo -------升级失败------
) else (
    echo -------升级成功------
)

pause

