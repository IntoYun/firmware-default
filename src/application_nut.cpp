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

/*
 * 智能灯程序
 * 硬件准备：  使用跳线帽把LS与A0相连
 */

#include "main.h"

#if PLATFORM_ID == PLATFORM_NUT

const pinmap_t pinMap[PIN_MAP_NUM] = {
    {"A0", A0}, {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4}, {"D5", D5}, {"D6", D6},\
    {"RXD", RXD}, {"TXD", TXD}
};

#define SMARTLIGHT_CMD_SWITCH           "channel/smartLight_0/cmd/switch"       //开关命令
#define SMARTLIGHT_DATA_STATUS          "channel/smartLight_0/data/status"      //开关状态

#define LEDPIN                          LED_BUILTIN      //定义灯泡控制引脚
#define SENSORPIN                       A0               //定义光传感器引脚

#define DPID_BOOL_SWITCH                1  //布尔型            开关
#define DPID_DOUBLE_ILLUMINATION        2  //数值型            光照强度

bool dpBoolSwitch;                      // 开关
double dpDoubleIllumination;            // 光照强度
uint32_t timerID;

//兼容intorobot
void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1') {
        digitalWrite(LEDPIN, LOW);     // 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    } else {
        digitalWrite(LEDPIN, HIGH);     // 关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    //if ((event == event_cloud_data) && (param == ep_cloud_comm_data)) {
    if ((event == event_cloud_data) && (param == ep_cloud_data_datapoint)) {
        if (RESULT_DATAPOINT_NEW == Cloud.readDatapoint(DPID_BOOL_SWITCH, dpBoolSwitch)) {
            if(dpBoolSwitch) {
                digitalWrite(LEDPIN, LOW);    // 打开灯泡
            } else {
                digitalWrite(LEDPIN, HIGH);   // 关闭灯泡
            }
        }
    }
}

void userInit(void)
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    pinMode(SENSORPIN, AN_INPUT);

    //兼容intorobot
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);

    //定义数据点事件
    System.on(event_cloud_data, system_event_callback);
    //定义产品数据点
    Cloud.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false); //开关
    Cloud.defineDatapointNumber(DPID_DOUBLE_ILLUMINATION, DP_PERMISSION_UP_ONLY, 0, 10000, 1, 0); //光照强度

    timerID = timerGetId();
}

void userHandle(void)
{
    if(timerIsEnd(timerID, 10000)) {
        timerID = timerGetId();

        int data = analogRead(SENSORPIN);
        if (data == 0) {
            dpDoubleIllumination = 0.0;
        } else {
            dpDoubleIllumination = -2.712e-08 * data * data * data - 5.673e-05 * data * data  + 1.788 * data + 122.1;
        }
        //更新数据点数据（数据点具备上送属性）
        Cloud.writeDatapoint(DPID_DOUBLE_ILLUMINATION, dpDoubleIllumination);
    }
}

#endif

