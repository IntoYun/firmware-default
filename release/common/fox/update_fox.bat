@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe
set DFU_TOOL=tools/dfu-util/win/dfu-util.exe


:start

echo -------Fox�̼�����--------
echo ��ѡ����������(�س�Ĭ��ѡ��1):
echo 1. ����stm32    (��ȷ������ST-LINK)
echo 2. ����SPI Flash  (��ȷ�������Ѱ�װDFU����������Fox�ѽ���DFU����ģʽ)

@set select_type=1
set /p select_type=
if %select_type%==2 (
goto spi_flash_update_start
)
goto stm32_update_start

:spi_flash_update_start
echo spi flash ������...
echo %DFU_TOOL%
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D default-fox.dfu

goto update_end

:stm32_update_start

echo stm32   ������...
echo %ST_TOOL%
"%ST_TOOL%" write fox-boot.bin 0x8000000

:: ��ʱ1s
choice /t 1 /d y /n >nul
"%ST_TOOL%" --reset write default-fox.bin 0x08020000

:update_end

if errorlevel 1 (
    echo -------����ʧ��------
) else (
    echo -------�����ɹ�------
)

pause

