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
"%DFU_TOOL%" -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D firmware.dfu
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
)

goto update_end

:stm32_update_start

echo 1.����Bootloader ...
"%ST_TOOL%" write boot-v1.bin 0x8000000
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
goto update_end
)

echo 2.����Ӧ�ó��� ...
"%ST_TOOL%" --reset write firmware.bin 0x08020000
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

