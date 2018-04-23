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
    SUDO=
else    #linux
    #下载工具
    ST_TOOL=./tools/stlink/linux64/st-flash
    SUDO=sudo
fi

cecho "-------ant软件升级--------" $yellow
cecho "1.下载bootloader ... \c" $green
$SUDO $ST_TOOL write boot-v1.bin 0x8000000
if [ $? = 0 ]; then
    cecho "成功" $yellow
    sleep 1
    cecho "2.下载应用程序   ... \c" $green
    $SUDO $ST_TOOL --reset write firmware.bin 0x08006000
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

if [ $result = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

