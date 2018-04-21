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

#if PLATFORM_ID == PLATFORM_ATOM

PRODUCT_ID(1ef8893400000355)
PRODUCT_SECRET(46a4fa8496c8521185b9e4c824c9c08c)
PRODUCT_SOFTWARE_VERSION(1.0.0)

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

void userHandle (void)
{
}

#endif

