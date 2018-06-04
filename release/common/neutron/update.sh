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
    ESP8266_TOOL=./tools/esp8266/osx/esptool
    ST_TOOL=./tools/stlink/osx/st-flash
    DFU_TOOL=./tools/dfu-util/osx/dfu-util
    DFUSUFFIX_TOOL=./tools/dfu-util/osx/dfu-suffix
    SUDO=
    #下载参数
    UPLOAD_SPEED=230400
    UPLOAD_PORT=/dev/cu.usbmodem1411

else
    #下载工具
    ESP8266_TOOL=./tools/esp8266/linux64/esptool
    ST_TOOL=./tools/stlink/linux64/st-flash
    DFU_TOOL=./tools/dfu-util/linux64/dfu-util
    DFUSUFFIX_TOOL=./tools/dfu-util/osx/dfu-suffix
    SUDO=sudo
    #下载参数
    UPLOAD_SPEED=460800
    UPLOAD_PORT=/dev/ttyACM0
fi

cecho "-------IntoRobot Neutron软件升级--------" $yellow
cecho "-->请选择下载内容(回车默认选择1):                  " $yellow
cecho "1. 应用程序      (请手动进入DFU模式)               " $yellow
cecho "2. 完整包        (请确保连接ST-LINK)               " $yellow
cecho "3. 升级esp8266   (请确保neutron进入esp8266升级模式)" $yellow
cecho "其他退出                                           " $yellow

read select_type
if [ -z "${select_type}" ];then
  select_type=1
fi

case "$select_type" in
    1)
        cp firmware.bin firmware.dfu
        $DFUSUFFIX -v 0483 -p df11 -a firmware.dfu &>/dev/null
        cecho "下载应用程序 ... \c" $green
        $SUDO $DFU_TOOL -d 0x0483:0xdf11 -a 0 -R -s 0x08020000:leave -D firmware.dfu
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
        $SUDO $ST_TOOL write boot-v3.bin 0x8000000
        if [ $? = 0 ]; then
            cecho "成功" $yellow
            sleep 1
            cecho "下载应用程序   ... \c" $green
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

    3)
        if [ $sysType = "Darwin" ]; then    # osx
            cecho "-->请输入串口(回车默认输入/dev/cu.usbmodem1411): " $yellow
        else
            cecho "-->请输入串口(回车默认输入/dev/ttyACM0): " $yellow
        fi
        read usart_port
        if [ "$usart_port" != "" ];then
            UPLOAD_PORT=$usart_port
        fi
        if [ $sysType = "Darwin" ]; then    # osx
            cecho "-->请选择串口波特率(回车默认选择2.230400): " $yellow
        else
            cecho "-->请选择串口波特率(回车默认选择3.460800): " $yellow
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

        cecho "esp8266 升级中..." $green
        $SUDO $ESP8266_TOOL -v -cd nodemcu -cp $UPLOAD_PORT -cb $UPLOAD_SPEED -ca 0x00000 -cf net-boot.bin -ca 0x08000 -cf net.bin -ca 0xBC000 -cf firmware.bin -ca 0xFC000 -cf blank.bin -ca 0x100000 -cf boot-v3.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin
        if [ $? = 0 ]; then
            result=0
            cecho "成功" $yellow
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

