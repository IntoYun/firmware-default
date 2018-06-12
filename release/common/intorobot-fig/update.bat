@echo off
::���ع���
set ESP32_TOOL=tools/esp32/win/esptool.exe
::���ز���
set UPLOAD_SPEED=921600
set UPLOAD_PORT=COM1

::fig flash����
set FLASH_SIZE=4MB
set FLASH_MODE=qio
set FLASH_SPEED=40m

:start

echo -------IntoRobot Fig�������--------

set usart_port=%UPLOAD_PORT%
set /p usart_port=�����봮��(�س�Ĭ������COM1):
set UPLOAD_PORT=%usart_port%

echo ��ѡ�񴮿ڲ�����(�س�Ĭ��ѡ��4.921600):
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

:update_start
echo esp32 ������...

"%ESP32_TOOL%" --chip esp32 --port %UPLOAD_PORT% --baud %UPLOAD_SPEED% --before default_reset --after hard_reset write_flash -z --flash_freq %FLASH_SPEED% --flash_mode %FLASH_MODE% --flash_size %FLASH_SIZE% 0x1000 boot-v3.bin 0x8000 partitions.bin 0x9000 blank.bin 0x20000 firmware.bin 0x120000 firmware.bin 0x219000 boot-v3.bin

if errorlevel 1 (
    echo -------����ʧ��------
) else (
    echo -------�����ɹ�------
)

pause

