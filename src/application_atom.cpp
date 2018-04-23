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
 */
#include "main.h"

#if PLATFORM_ID == PLATFORM_ATOM

const pinmap_t pinMap[PIN_MAP_NUM] = {
    {"A0", A0}, {"A1", A1}, {"A2", A2}, {"A3", A3}, {"A4", A4}, {"A5", A5}, {"A6", A6}, {"A7", A7}, {"A8", A8},\
    {"A9", A9}, {"A10", A10}, {"A11", A11}, {"A12", A12}, {"A13", A13}, {"A14", A14}, {"A15", A15},\
    {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4}, {"D5", D5}, {"D6", D6}, {"D7", D7}, {"D8", D8}, {"D9", D9},\
    {"D10", D10}, {"D11", D11}, {"D12", D12}, {"D13", D13}, {"D14", D14}, {"D15", D15}, {"D16", D16}, {"D17", D17}, {"D18", D18}, {"D19", D19},\
    {"D20", D20}, {"D21", D21}, {"D22", D22}, {"D23", D23}, {"D24", D24}
};


#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#define LEDPIN                   LED_BUILTIN  //定义灯泡控制引脚

#define DPID_BOOL_SWITCH         1            //布尔型            开关

bool dpBoolSwitch;   // 开关

//兼容intorobot
void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1') {
        digitalWrite(LEDPIN, HIGH);     // 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    } else {
        digitalWrite(LEDPIN, LOW);     // 关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    //if ((event == event_cloud_data) && (param == ep_cloud_comm_data)) {
    if ((event == event_cloud_data) && (param == ep_cloud_data_datapoint)) {
        if (RESULT_DATAPOINT_NEW == Cloud.readDatapoint(DPID_BOOL_SWITCH, dpBoolSwitch)) {
            if(dpBoolSwitch) {
                digitalWrite(LEDPIN, HIGH);    // 打开灯泡
            } else {
                digitalWrite(LEDPIN, LOW);     // 关闭灯泡
            }
        }
    }
}

void userInit(void)
{
    //初始化
    pinMode(LEDPIN, OUTPUT);

    //兼容intorobot
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);

    //定义数据点事件
    System.on(event_cloud_data, system_event_callback);
    //定义产品数据点
    Cloud.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false); //开关
}

void userHandle(void)
{
}

#endif

