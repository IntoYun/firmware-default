@echo off
::���ع���
set ST_TOOL=tools/stlink/win/st-flash.exe


:start

echo -------IntoRobot Atom�̼�����--------
echo ��ѡ����������(�س�Ĭ��ѡ��1):
echo 1. ������    (��ȷ������ST-LINK)

pause

echo 1.����Ӧ�ó��� ...
"%ST_TOOL%" --reset write firmware.bin 0x08000000
if %errorlevel%==0 (
echo �ɹ�
) else (
echo ʧ��
)

if errorlevel 0 (
    echo -------���سɹ�------
) else (
    echo -------����ʧ��------
)

pause

