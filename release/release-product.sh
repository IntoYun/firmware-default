#! /bin/bash
# Author: chenkaiyao <chenkaiyao@molmc.com>
source ./release-version.sh

# Color-echo.
# arg $1 = message
# arg $2 = Color
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

RELEASE_DIR=$(cd "$(dirname "$0")"; pwd)
RELEASE_PRODUCT_DIR=$RELEASE_DIR/package
RELEASE_COMMON_DIR=$RELEASE_DIR/common

# $1:产品名称 $2:产品软件版本号
release_product() {
    cecho "开始生成产品软件包 - $1(版本:$2)" $yellow
    cecho "---------------------------------------------------------------------------" $yellow

    RELEASE_PRODUCT_ITEM_DIR=$RELEASE_PRODUCT_DIR/$1
    RELEASE_PRODUCT_VERSION_DIR=$RELEASE_PRODUCT_ITEM_DIR/$1-$2

    rm -rf $RELEASE_PRODUCT_ITEM_DIR
    mkdir -p $RELEASE_PRODUCT_VERSION_DIR/tools

    cd $RELEASE_DIR

    intoyuniot run -d .. -e intorobot-$1 -t clean
    intoyuniot run -d .. -e intorobot-$1
    cp ../.intoiotenvs/intorobot-$1/firmware.bin $RELEASE_PRODUCT_VERSION_DIR

    if [ "$1"x = "atom-old"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp8266 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "neutron"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp8266 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "nut"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "fig"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "ant"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "w67"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "w323"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    elif [ "$1"x = "l6"x ];then
        cp -rf $RELEASE_COMMON_DIR/tools/esp32 $RELEASE_PRODUCT_VERSION_DIR/tools

    fi
    cp -rf $RELEASE_COMMON_DIR/$1/* $RELEASE_PRODUCT_VERSION_DIR
}

if [ $# != 1 ];then
cecho "请选择要生成软件包的型号: " $yellow
cecho "0. 全部项目 "   $yellow
cecho "1. yunslave-w67   (版本:$PRODUCT_VERSION_STRING_YUNSLAVE_W67)   "   $yellow
cecho "2. yunslave-w323  (版本:$PRODUCT_VERSION_STRING_YUNSLAVE_W323)  "   $yellow
cecho "其他退出                                                        "   $yellow
read type
else
type=$1
fi

find $RELEASE_DIR -name ".DS_Store" -delete
case $type in
    0 )  # all
        release_product yunslave-w67 $PRODUCT_VERSION_STRING_YUNSLAVE_W67
        release_product yunslave-w323 $PRODUCT_VERSION_STRING_YUNSLAVE_W323
        ;;

    1 )  # yunslave-w67
        release_product yunslave-w67 $PRODUCT_VERSION_STRING_YUNSLAVE_W67
        ;;

    2 )  # yunslave-w323
        release_product yunslave-w323 $PRODUCT_VERSION_STRING_YUNSLAVE_W323
        ;;

    * )
        exit 0
        ;;
esac

cecho "打包完成!!!" $green
exit 0

