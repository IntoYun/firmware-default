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
    ESP8266_TOOL=./tools/esp8266/osx/esptool
    ST_TOOL=./tools/stlink/osx/st-flash
    SUDO=
    #下载参数
    UPLOAD_SPEED=230400
    UPLOAD_PORT=/dev/cu.usbmodem1411
else    #linux
    #下载工具
    ESP8266_TOOL=./tools/esp8266/linux64/esptool
    ST_TOOL=./tools/stlink/linux64/st-flash
    SUDO=sudo
    #下载参数
    UPLOAD_SPEED=460800
    UPLOAD_PORT=/dev/ttyACM0
fi


cecho "-------neutron软件升级--------" $yellow
cecho "-->请选择升级内容(回车默认选择1): " $yellow
cecho "1. 升级stm32    (请确保连接ST-LINK)" $yellow
cecho "2. 升级esp8266  (请确保neutron进入esp8266升级模式)" $yellow

read select_type

case "$select_type" in
    2)
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
        $SUDO $ESP8266_TOOL -v -cd nodemcu -cp $UPLOAD_PORT -cb $UPLOAD_SPEED -ca 0x00000 -cf neutron-net-boot.bin -ca 0x08000 -cf neutron-net.bin -ca 0xBC000 -cf default-neutron.bin -ca 0xFC000 -cf blank.bin -ca 0x100000 -cf neutron-boot.bin -ca 0x3FC000 -cf blank.bin -ca 0x3FC000 -cf esp_init_data_default.bin
        ;;
    *)
        cecho "stm32   升级中..." $green
        $SUDO $ST_TOOL write neutron-boot.bin 0x8000000
        sleep 1
        $SUDO $ST_TOOL --reset write default-neutron.bin 0x08020000
        ;;
esac

if [ $? = 0 ]; then
    cecho "-------升级成功------" $yellow
else
    cecho "-------升级失败------" $red
fi

