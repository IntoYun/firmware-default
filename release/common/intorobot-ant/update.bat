@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe
set DFU_TOOL=tools/dfu-util/win/dfu-util.exe
set DFUSUFFIX=tools/dfu-util/win/dfu-suffix.exe

:start

echo -------IntoRobot Ant�������--------
echo ��ѡ����������(�س�Ĭ��ѡ��1):
echo 1. Ӧ�ó���  (���ֶ�����DFUģʽ)
echo 2. ������    (��ȷ������ST-LINK)

set select_type=1
set /p select_type=
if %select_type%==1 (
goto app_update
)
goto all_update_start

:app_update

copy firmware.bin firmware.dfu >nul 2>nul
"%DFUSUFFIX%" -v 0483 -p df11 -a firmware.dfu >nul 2>nul
set /p=����Ӧ�ó���   ... <nul
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x08006000:leave -D firmware.dfu
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
)
goto update_end

:all_update_start

set /p=����Bootloader ... <nul
"%ST_TOOL%" write boot-v2.bin 0x8000000
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
goto update_end
)

set /p=����Ӧ�ó���   ... <nul
:: ��ʱ1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write firmware.bin 0x08006000
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
)

:update_end

if errorlevel 0 (
    echo -------���سɹ�------
) else (
    echo -------����ʧ��------
)

pause

