@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------L6�̼�����--------
echo ����stm32    (��ȷ������ST-LINK)

echo stm32   ������...
echo %ST_TOOL%
"%ST_TOOL%" write l6-boot.bin 0x8000000

:: ��ʱ1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-l6.bin 0x08006000

:update_end

if errorlevel 1 (
    echo -------����ʧ��------
) else (
    echo -------�����ɹ�------
)

pause

