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
    ST_TOOL=./tools/stlink/osx/st-flash
    DFU_TOOL=./tools/dfu-util/osx/dfu-util
    SUDO=
    #下载参数
    UPLOAD_PORT=/dev/cu.usbmodem1411
else    #linux
    #下载工具
    ST_TOOL=./tools/stlink/linux64/st-flash
    DFU_TOOL=./tools/dfu-util/linux64/dfu-util
    SUDO=sudo
    #下载参数
    UPLOAD_PORT=/dev/ttyACM0
fi

cecho "-------fox软件升级--------" $yellow
cecho "-->请选择升级内容(回车默认选择1): " $yellow
cecho "1. 升级stm32     (请确保连接ST-LINK)" $yellow
cecho "2. 升级spi-flash (请手动进入DFU模式) " $yellow

read select_type

case "$select_type" in
    1)
        cecho "stm32   升级中..." $green
        cecho "1. 下载bootloader ... \c" $green
        $SUDO $ST_TOOL write boot-v1.bin 0x8000000
        if [ $? = 0 ]; then
            cecho "成功" $yellow
            sleep 1
            cecho "2. 下载应用程序   ... \c" $green
            $SUDO $ST_TOOL --reset write firmware.bin 0x08020000
            if [ $? = 0 ]; then
                result=0
                cecho "成功" $yellow
            else
                result=-1
                cecho "失败" $red
            fi
        else
            result=-1
            cecho "失败" $red
        fi
        ;;
    2)
        cecho "spi-flash   升级中..." $green
        $SUDO $DFU_TOOL -d 0x0483:0xdf11 -a 0 -R -s 0x09000000:leave -D firmware.dfu
        if [ $? = 0 ]; then
            result=0
            cecho "成功" $yellow
        else
            result=-1
            cecho "失败" $red
        fi
        ;;
    *)
        ;;
esac

if [ $result = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

