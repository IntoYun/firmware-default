@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------Ant�̼�����--------
echo ����stm32    (��ȷ������ST-LINK)

echo stm32   ������...
echo %ST_TOOL%
"%ST_TOOL%" write ant-boot.bin 0x8000000

:: ��ʱ1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-ant.bin 0x08006000

:update_end

if errorlevel 1 (
    echo -------����ʧ��------
) else (
    echo -------�����ɹ�------
)

pause

