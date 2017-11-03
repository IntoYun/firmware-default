@echo off
::���ع���
set ESP8266_TOOL=tools/esp8266/win/esptool.exe
set ST_TOOL=tools/stlink/win/st-flash.exe
::���ز���
set UPLOAD_SPEED=460800
set UPLOAD_PORT=COM1

:start

echo -------neutron�������--------
echo ��ѡ����������(�س�Ĭ��ѡ��1):
echo 1. ����stm32    (��ȷ������ST-LINK)
echo 2. ����esp8266  (��ȷ��neutron����esp8266����ģʽ)
@set select_type=1
set /p select_type=
if %select_type%==2 (
goto esp8266_update
)
goto stm32_update_start

:esp8266_update

set usart_port=%UPLOAD_PORT%
set /p usart_port=�����봮��(�س�Ĭ������COM1):
set UPLOAD_PORT=%usart_port%

echo ��ѡ�񴮿ڲ�����(�س�Ĭ��ѡ��3.460800):
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
echo esp8266 ������...
"%ESP8266_TOOL%" -v -cd nodemcu -cp %UPLOAD_PORT% -cb %UPLOAD_SPEED% -ca 0x00000 -cf neutron-net-boot.bin -ca 0x08000 -cf neutron-net.bin -ca 0xBC000 -cf default-neutron.bin -ca 0xFC000 -cf blank.bin -ca 0x100000 -cf neutron-boot.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin

goto update_end

:stm32_update_start

echo stm32   ������...
echo %ST_TOOL%
"%ST_TOOL%" write neutron-boot.bin 0x8000000

:: ��ʱ1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-neutron.bin 0x08020000

:update_end

if errorlevel 1 (
    echo -------����ʧ��------
) else (
    echo -------�����ɹ�------
)

pause

