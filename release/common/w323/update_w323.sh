#! /bin/bash

cecho() {
  echo -e "${2}${1}"
  tput sgr0
  # Reset # Reset to normal.
  return
}
# Set the colours you can use
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'

#下载工具
sysType=`uname -s`
cecho "----您的系统是 $sysType ------" $green
if [ $sysType = "Darwin" ]; then    # osx
    #下载工具
    ESP32_TOOL=./tools/esp32/osx/esptool
    SUDO=
    #下载参数
    UPLOAD_SPEED=921600
    UPLOAD_PORT=/dev/cu.SLAB_USBtoUART
else    #linux
    #下载工具
    ESP32_TOOL=./tools/esp32/linux64/esptool
    SUDO=sudo
    #下载参数
    UPLOAD_SPEED=921600
    UPLOAD_PORT=/dev/ttyUSB0
fi

#fig flash参数
FLASH_SIZE=4MB
FLASH_MODE=qio
FLASH_SPEED=40m

cecho "-------w323软件升级--------" $yellow
if [ $sysType = "Darwin" ]; then    # osx
    cecho "-->请输入串口(回车默认输入/dev/cu.SLAB_USBtoUART): " $yellow
else
    cecho "-->请输入串口(回车默认输入/dev/ttyUSB0): " $yellow
fi
read usart_port
if [ "$usart_port" != "" ];then
    UPLOAD_PORT=$usart_port
fi
if [ $sysType = "Darwin" ]; then    # osx
    cecho "-->请选择串口波特率(回车默认选择4.921600): " $yellow
else
    cecho "-->请选择串口波特率(回车默认选择4.921600): " $yellow
fi
cecho "1. 115200  2. 230400  3. 460800  4. 921600" $yellow
read usart_speed
case "$usart_speed" in
    1)
        UPLOAD_SPEED=115200
        ;;
    2)
        UPLOAD_SPEED=230400
        ;;
    3)
        UPLOAD_SPEED=460800
        ;;
    4)
        UPLOAD_SPEED=921600
        ;;
    *)
        ;;
esac

cecho "esp32 升级中..." $green
$SUDO $ESP32_TOOL --chip esp32 --port $UPLOAD_PORT --baud $UPLOAD_SPEED --before default_reset --after hard_reset write_flash -z --flash_freq $FLASH_SPEED --flash_mode $FLASH_MODE --flash_size $FLASH_SIZE 0x1000 w323-boot.bin 0x8000 partitions.bin 0x9000 blank.bin 0x20000 default-w323.bin

if [ $? = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

