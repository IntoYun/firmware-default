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
    UPLOAD_RESET_TOOL=./tools/upload-reset/osx/upload-reset
    ST_TOOL=./tools/stlink/osx/st-flash
    DFU_TOOL=./tools/dfu-util/osx/dfu-util
    SUDO=
    #下载参数
    UPLOAD_PORT=/dev/cu.usbmodem1411
else    #linux
    #下载工具
    UPLOAD_RESET_TOOL=./tools/upload-reset/linux64/upload-reset
    ST_TOOL=./tools/stlink/linux64/st-flash
    DFU_TOOL=./tools/dfu-util/linux64/dfu-util
    SUDO=sudo
    #下载参数
    UPLOAD_PORT=/dev/ttyACM0
fi

cecho "-------fox软件升级--------" $yellow
cecho "-->请选择升级内容(回车默认选择1): " $yellow
cecho "1. 升级stm32    (请确保连接ST-LINK)" $yellow
cecho "2. 升级spi-flash                   " $yellow

read select_type

case "$select_type" in
    1)
        cecho "stm32   升级中..." $green
        $SUDO $ST_TOOL write fox-boot.bin 0x8000000
        sleep 1
        $SUDO $ST_TOOL --reset write default-fox.bin 0x08020000
        ;;
    2)
        cecho "spi-flash   升级中..." $green
        if [ $sysType = "Darwin" ]; then    # osx
            cecho "-->请输入串口(回车默认输入/dev/cu.usbmodem1411): " $yellow
        else
            cecho "-->请输入串口(回车默认输入/dev/ttyACM0): " $yellow
        fi
        read usart_port
        if [ "$usart_port" != "" ];then
            UPLOAD_PORT=$usart_port
        fi
        cecho "进入下载模式..." $green
        $UPLOAD_RESET_TOOL -p $UPLOAD_PORT -b 19200 -t 2000
        $SUDO $DFU_TOOL -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D default-fox.dfu
        ;;
    *)
        ;;
esac

if [ $? = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

