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
 * 硬件准备：  使用跳线帽把LS与A5相连
 */

#if PLATFORM_ID == PLATFORM_ANT

PRODUCT_ID(ddab5c4700000359)                        // 产品标识
PRODUCT_SECRET(c93bbb81ec731fbbaea5625fe12271e1)    // 产品密钥
PRODUCT_SOFTWARE_VERSION(1.0.0)                     // 产品软件版本号

#define DPID_BOOL_SWITCH                          1  //布尔型            开关
#define DPID_DOUBLE_TEMPERATURE                   2  //数值型            温度
#define DPID_INT32_HUMIDITY                       3  //数值型            湿度
#define DPID_DOUBLE_ILLUMINATION                  4  //数值型            光照强度

#define LEDPIN                          LED_BUILTIN      //定义灯泡控制引脚
#define SENSORPIN                       A5               //定义光传感器引脚

bool dpBoolSwitch;                      // 开关
double dpDoubleTemperature = 25.0;      // 温度
int32_t dpInt32Humidity = 56;           // 湿度
double dpDoubleIllumination;            // 光照强度
uint32_t timerID;

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    //if ((event == event_cloud_data) && (param == ep_cloud_comm_data)) {
    if ((event == event_cloud_data) && (param == ep_cloud_data_datapoint)) {
        /*************此处修改和添加用户控制代码*************/
        if (RESULT_DATAPOINT_NEW == Cloud.readDatapoint(DPID_BOOL_SWITCH, dpBoolSwitch)) {
            if(dpBoolSwitch) {
                digitalWrite(LEDPIN, LOW);    // 打开灯泡
            } else {
                digitalWrite(LEDPIN, HIGH);   // 关闭灯泡
            }
        }
        Cloud.sendDatapointAll(0, 120);
        /*******************************************************/
    }
}

void userInit(void)
{
    //定义数据点事件
    System.on(event_cloud_data, system_event_callback);

    //根据网关参数具体设置
    LoRaWan.setDataRate(DR_3);
    LoRaWan.setChannelStatus(0, false);               //关闭通道0 频率固定：433175000
    LoRaWan.setChannelStatus(1, false);               //关闭通道1 频率固定：433375000
    LoRaWan.setChannelStatus(2, true);                //打开通道2 频率固定：433575000
    LoRaWan.setChannelFreq(2, 433575000);             //设置通道2频率
    LoRaWan.setChannelDRRange(2, DR_3, DR_3);         //设置通道2速率范围
    LoRaWan.setChannelStatus(3, false);               //关闭通道3

    //定义产品数据点
    Cloud.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false); //开关
    Cloud.defineDatapointNumber(DPID_DOUBLE_TEMPERATURE, DP_PERMISSION_UP_ONLY, 0, 100, 1, 0); //温度
    Cloud.defineDatapointNumber(DPID_INT32_HUMIDITY, DP_PERMISSION_UP_ONLY, 0, 100, 0, 0); //湿度
    Cloud.defineDatapointNumber(DPID_DOUBLE_ILLUMINATION, DP_PERMISSION_UP_ONLY, 0, 10000, 1, 0); //光照强度

    /*************此处修改和添加用户初始化代码**************/
    //初始化
    pinMode(LEDPIN, OUTPUT);
    pinMode(SENSORPIN, AN_INPUT);

    timerID = timerGetId();
    /*******************************************************/
}

void userHandle (void)
{
    /*************此处修改和添加用户处理代码****************/
    if(Cloud.connected() < 0) {
        if(Cloud.connect(JOIN_OTAA, 400) == 0) {
            LoRaWan.setMacClassType(CLASS_C);    //入网成功后设置为A/C类
        }
    } else if(Cloud.connected() == 0) {
        if(timerIsEnd(timerID, 20000)) {
            timerID = timerGetId();

            int data = analogRead(SENSORPIN);
            if (data == 0) {
                dpDoubleIllumination = 0.0;
            } else {
                dpDoubleIllumination = -2.712e-08 * data * data * data - 5.673e-05 * data * data  + 1.788 * data + 122.1;
            }
            //更新数据点数据（数据点具备上送属性）
            Cloud.writeDatapoint(DPID_DOUBLE_TEMPERATURE, dpDoubleTemperature);
            Cloud.writeDatapoint(DPID_INT32_HUMIDITY, dpInt32Humidity);
            Cloud.writeDatapoint(DPID_DOUBLE_ILLUMINATION, dpDoubleIllumination);

            //发送数据点数据，建议不频繁上送数据
            Cloud.sendDatapointAll(0, 120);
        }
    }
    /*******************************************************/
}

#endif

