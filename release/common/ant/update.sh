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
    ST_TOOL=./tools/stlink/osx/st-flash
    DFU_TOOL=./tools/dfu-util/osx/dfu-util
    DFUSUFFIX_TOOL=./tools/dfu-util/osx/dfu-suffix
    SUDO=
else
    #下载工具
    ST_TOOL=./tools/stlink/linux64/st-flash
    DFU_TOOL=./tools/dfu-util/linux64/dfu-util
    DFUSUFFIX_TOOL=./tools/dfu-util/osx/dfu-suffix
    SUDO=sudo
fi

cecho "-------IntoRobot Ant软件升级--------" $yellow
cecho "-->请选择下载内容(回车默认选择1):       " $yellow
cecho "1. 应用程序   (请手动进入DFU模式)       " $yellow
cecho "2. 完整包     (请确保连接ST-LINK)       " $yellow
cecho "其他退出                                " $yellow
select_type=1
read select_type

case "$select_type" in
    1)
        cp firmware.bin firmware.dfu
        $DFUSUFFIX -v 0483 -p df11 -a firmware.dfu &>/dev/null
        cecho "下载应用程序 ... \c" $green
        $SUDO $DFU_TOOL -d 0x0483:0xdf11 -a 0 -R -s 0x08006000:leave -D firmware.dfu
        if [ $? = 0 ]; then
            result=0
            cecho "成功" $yellow
        else
            result=-1
            cecho "失败" $red
        fi
        ;;

    2)
        cecho "下载bootloader ... \c" $green
        $SUDO $ST_TOOL write boot-v2.bin 0x8000000
        if [ $? = 0 ]; then
            cecho "成功" $yellow
            sleep 1
            cecho "下载应用程序   ... \c" $green
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

