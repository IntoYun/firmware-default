/**
******************************************************************************
Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see <http://www.gnu.org/licenses/>.
******************************************************************************
*/

#include "Arduino.h"

/*
 * 智能灯程序
 */

#if PLATFORM_ID == PLATFORM_W67 || PLATFORM_ID == PLATFORM_W323

PRODUCT_ID(QEeKboWS7GXnS19a)
PRODUCT_SECRET(d9af6c4db7572c8b711f5384ad4219ff)
PRODUCT_SOFTWARE_VERSION(1.0.0)
PRODUCT_HARDWARE_VERSION(1.0.0)

#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#if PLATFORM_ID == PLATFORM_W67
#define KEYPIN    GPIO0     //定义按键引脚
#define LEDPIN    GPIO16    //定义灯泡控制引脚
#elif PLATFORM_ID == PLATFORM_W323
#define KEYPIN    GPIO0     //定义按键引脚
#define LEDPIN    GPIO4     //定义灯泡控制引脚
#endif

void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1') {
        digitalWrite(LEDPIN, LOW);     //nut 低电平 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    } else {
        digitalWrite(LEDPIN, HIGH);      //nut 高电平 关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void systemConfigKeyDeal()
{
    if(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL != System.configCurrentMode()) {
        System.configEnterMode(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL);
        digitalWrite(LEDPIN, LOW);
    } else {
        System.configExit();
        digitalWrite(LEDPIN, HIGH);
    }
}

void setup()
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    if(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL == System.configCurrentMode()) {
        digitalWrite(LEDPIN, LOW);
    } else {
        digitalWrite(LEDPIN, HIGH);
    }
    attachInterrupt(KEYPIN, systemConfigKeyDeal, CHANGE);
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);
}

void loop()
{
    digitalWrite(LEDPIN,1);
    delay(1000);
    digitalWrite(LEDPIN,0);
    delay(1000);
}

#elif PLATFORM_ID == PLATFORM_ANT || PLATFORM_ID == PLATFORM_L6

void setup()
{
}

void loop()
{
}

#else

#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#define LEDPIN    LED_BUILTIN    //定义灯泡控制引脚

void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1')
    {
#if PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_NEUTRON
        digitalWrite(LEDPIN, HIGH);     // 打开灯泡
#else
        digitalWrite(LEDPIN, LOW);     // 打开灯泡
#endif
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    }
    else
    {
#if PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_NEUTRON
        digitalWrite(LEDPIN, LOW);     // 打开灯泡
#else
        digitalWrite(LEDPIN, HIGH);     // 打开灯泡
#endif
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void setup()
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);
}

void loop()
{
}

#endif
