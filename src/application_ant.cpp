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

#include "main.h"

#if PLATFORM_ID == PLATFORM_ANT

const pinmap_t pinMap[PIN_MAP_NUM] = {
    {"A0", A0}, {"A1", A1}, {"A2", A2}, {"A3", A3}, {"A4", A4}, {"A5", A5},\
    {"D0", D0}, {"D1", D1}, {"D2", D2}, {"D3", D3}, {"D4", D4}, {"D5", D5}, {"D6", D6},\
    {"RXD", RXD}, {"TXD", TXD}
};

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

void eventProcess(system_event_t event, int param, uint8_t *data, uint16_t datalen)
{
    if(event == event_cloud_comm) {
        switch(param) {
            case ep_cloud_comm_data:
                if (RESULT_DATAPOINT_NEW == Cloud.readDatapoint(DPID_BOOL_SWITCH, dpBoolSwitch)) {
                    if(dpBoolSwitch) {
                        digitalWrite(LEDPIN, LOW);    // 打开灯泡
                    } else {
                        digitalWrite(LEDPIN, HIGH);   // 关闭灯泡
                    }
                }
            default:
                break;
        }
    }
}

void userInit(void)
{
    //定义数据点事件
    System.on(event_all, eventProcess);
    //根据网关参数具体设置
    LoRaWan.setDataRate(DR_3);
    LoRaWan.setChannelStatus(0, false);               //关闭通道0 频率固定：433175000
    LoRaWan.setChannelStatus(1, false);               //关闭通道1 频率固定：433375000
    LoRaWan.setChannelStatus(2, true);                //打开通道2 频率固定：433575000
    LoRaWan.setChannelFreq(2, 433575000);             //设置通道2频率
    LoRaWan.setChannelDRRange(2, DR_3, DR_3);         //设置通道2速率范围
    LoRaWan.setChannelStatus(3, false);               //关闭通道3
    LoRaWan.setDutyCyclePrescaler(1);                 //设置占空比
    //定义产品数据点
    Cloud.defineDatapointBool(DPID_BOOL_SWITCH, DP_PERMISSION_UP_DOWN, false); //开关
    Cloud.defineDatapointNumber(DPID_DOUBLE_TEMPERATURE, DP_PERMISSION_UP_ONLY, 0, 100, 1, 0); //温度
    Cloud.defineDatapointNumber(DPID_INT32_HUMIDITY, DP_PERMISSION_UP_ONLY, 0, 100, 0, 0); //湿度
    Cloud.defineDatapointNumber(DPID_DOUBLE_ILLUMINATION, DP_PERMISSION_UP_ONLY, 0, 10000, 1, 0); //光照强度
    /*************此处修改和添加用户初始化代码**************/
    pinMode(LEDPIN, OUTPUT);
    pinMode(SENSORPIN, AN_INPUT);
    digitalWrite(LEDPIN, HIGH);    // 默认关闭灯泡

    timerID = timerGetId();
    /*******************************************************/
}

void userHandle(void)
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

bool rfCheck(double freq, char *datarate, int &tx_rssi, int &rx_rssi)
{
#define BUFFER_SIZE     8
    uint8_t txBuffer[BUFFER_SIZE] = {11,2,3,4,5,6,7,8}, rxBuffer[BUFFER_SIZE] = {0};
    uint8_t sf,bw;

    // datarate
    int x0, x1;
    if(sscanf(datarate, "SF%dBW%d", &x0, &x1) == 2) {
        switch (x0) {
            case  7: sf = 7;  break;
            case  8: sf = 8;  break;
            case  9: sf = 9;  break;
            case 10: sf = 10; break;
            case 11: sf = 11; break;
            case 12: sf = 12; break;
            default: sf = 7;  break;
        }
        switch (x1) {
            case 125: bw = 0; break;
            case 250: bw = 1; break;
            case 500: bw = 2; break;
            default: bw = 0; break;
        }
    }

    Cloud.setProtocol(PROTOCOL_P2P); //使用点对点通讯
    LoRa.radioSetFreq((uint32_t)(freq*1000000));         //设置频率
    LoRa.radioSetSF(sf);
    LoRa.radioSetBandwidth(bw);
    LoRa.radioSetIqInverted(true);

    if(LoRa.radioReadReg(0x42) != 0x12){
        goto failure;
    }
    if(LoRa.radioSend(txBuffer, BUFFER_SIZE, 2000) < 0) {
        goto failure;
    }

    LoRa.radioStartRx(0);
    if(LoRa.radioRx(rxBuffer, BUFFER_SIZE, rx_rssi) == BUFFER_SIZE) {
        if(rxBuffer[0] == 22) {
            tx_rssi = rxBuffer[1];
            return true;
        }
    }

failure:
    return false;

#undef BUFFER_SIZE
}

#endif

