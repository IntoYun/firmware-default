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

cecho "-------IntoRobot Atom软件升级--------" $yellow
cecho "-->请选择下载内容(回车默认选择1):    " $yellow
cecho "1. 完整包     (请确保连接ST-LINK)    " $yellow
cecho "其他退出                             " $yellow

read select_type
if [ -z "${select_type}" ];then
  select_type=1
fi

case "$select_type" in
    1)
        cecho "1.下载应用程序   ... \c" $green
        $SUDO $ST_TOOL --reset write firmware.bin 0x08000000
        if [ $? = 0 ]; then
            result=0
            cecho "成功" $yellow
        else
            result=-1
            cecho "失败" $red
        fi
        fi
        ;;

    *)
        exit 0
        ;;
esac

if [ $result = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

