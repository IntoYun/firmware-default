@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------Ant�̼�����--------
echo     (��ȷ������ST-LINK)

pause

echo 1.����Bootloader ...
"%ST_TOOL%" write boot-v1.bin 0x8000000
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
goto update_end
)

echo 2.����Ӧ�ó��� ...
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

