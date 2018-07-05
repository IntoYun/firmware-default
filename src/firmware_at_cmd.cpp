/**
******************************************************************************
Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

robin  2017-05-03

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

#include "firmware_config.h"
#include "firmware_at_cmd.h"
#include "firmware_at_parse.h"
#include "firmware_at.h"
#include "firmware_log.h"
#include "main.h"

const char *pinModeString[] = {"INPUT", "OUTPUT", "INPUT_PULLUP", "INPUT_PULLDOWN"};


void at_exeCmdNull(uint8_t id)
{
    log_v("at_exeCmdNull\r\n");
    at_response_ok();
}

void at_setupCmdE(uint8_t id, char *pPara)
{
    log_v("at_setupCmdE\r\n");
    if(*pPara == '0') {
        echoFlag = FALSE;
    } else if(*pPara == '1') {
        echoFlag = TRUE;
    } else {
        goto failure;
    }
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_exeCmdRst(uint8_t id)
{
    log_v("at_exeCmdRst\r\n");
    at_response_ok();
    delay(500);
    System.reset();
}

void at_exeCmdRestore(uint8_t id)
{
    log_v("at_exeCmdRestore\r\n");
    at_response_ok();
    delay(500);
    System.factoryReset();
}

void at_exeCmdConfig(uint8_t id)
{
    log_v("at_exeCmdConfig\r\n");
    at_response_ok();
}

void at_queryCmdInfo(uint8_t id)
{
    log_v("at_queryCmdInfo\r\n");
    String infoString = "";
    char buffer[17];
    int mode;

    infoString = "+INFO:\"";
    System.get(SYSTEM_PARAMS_PRODUCT_BOARD_NAME, buffer, sizeof(buffer));
    infoString += buffer;
    infoString += "_v";
    System.get(SYSTEM_PARAMS_PRODUCT_SOFTWARE_VERSION, buffer, sizeof(buffer));
    infoString += buffer;
    infoString += "\",\"";

    System.get(SYSTEM_PARAMS_PRODUCT_BOARD_NAME, buffer, sizeof(buffer));
    infoString += buffer;
    infoString += "\",\"";
    infoString += System.deviceID();
    infoString += "\",";

    System.get(SYSTEM_PARAMS_SECURITY_MODE, mode);
    infoString += mode;
    infoString += "\r\n";
    at_response(infoString.c_str());
    at_response_ok();
}

void at_setupCmdGetPinMode(uint8_t id, char *pPara)
{
    log_v("at_setupCmdGetPinMode\r\n");
    char pinName[16] = {0};
    int pin = -1;
    String respString = "";
    PinMode mode = OUTPUT;

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 Pin Name
    if(at_data_str_copy(pinName, &pPara, sizeof(pinName)) <= 0) {
        goto failure;
    }

    pin = pinNameToPin(pinName);
    if(pin < 0) {
        goto failure;
    }

    log_v("pinName = %s, pin = %d\r\n", pinName, pin);

    mode = getPinMode(pin);
    respString = "+GETPINMODE:";
    respString += (int)mode;
    respString += "\r\n";
    at_response(respString.c_str());
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_setupCmdSetPinMode(uint8_t id, char *pPara)
{
    log_v("at_setupCmdSetPinMode\r\n");
    char pinName[16] = {0};
    int pin = -1, mode;

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 Pin Name
    if(at_data_str_copy(pinName, &pPara, sizeof(pinName)) <= 0) {
        goto failure;
    }
    if (*pPara++ != ',') { // skip ','
        goto failure;
    }
    //获取第2个参数 mode
    if(!at_get_next_int_dec(&pPara, &mode)) {
        goto failure;
    }

    pin = pinNameToPin(pinName);
    if(pin < 0) {
        goto failure;
    }

    log_v("pinName = %s, pin = %d, pinMode = (%d)%s\r\n", pinName, pin, mode, pinModeString[mode]);
    pinMode(pin, mode);
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_setupCmdAnalogRead(uint8_t id, char *pPara)
{
    log_v("at_setupCmdAnalogRead\r\n");
    char pinName[16] = {0};
    int pin = -1;
    String respString = "";

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 Pin Name
    if(at_data_str_copy(pinName, &pPara, sizeof(pinName)) <= 0) {
        goto failure;
    }

    pin = pinNameToPin(pinName);
    if(pin < 0) {
        goto failure;
    }

    log_v("pinName = %s, pin = %d\r\n", pinName, pin);
    respString = "+ANALOGREAD:";
    respString += analogRead(pin);
    respString += "\r\n";
    at_response(respString.c_str());
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_setupCmdDigitalRead(uint8_t id, char *pPara)
{
    log_v("at_setupCmdDigitalRead\r\n");
    char pinName[16] = {0};
    int pin = -1;
    String respString = "";

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 Pin Name
    if(at_data_str_copy(pinName, &pPara, sizeof(pinName)) <= 0) {
        goto failure;
    }

    pin = pinNameToPin(pinName);
    if(pin < 0) {
        goto failure;
    }

    log_v("pinName = %s, pin = %d\r\n", pinName, pin);
    respString = "+DIGITALREAD:";
    respString += digitalRead(pin);
    respString += "\r\n";
    at_response(respString.c_str());
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_setupCmdDigitalWrite(uint8_t id, char *pPara)
{
    log_v("at_setupCmdDigitalWrite\r\n");
    char pinName[16] = {0};
    int pin = -1, value = 0;

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 Pin Name
    if(at_data_str_copy(pinName, &pPara, sizeof(pinName)) <= 0) {
        goto failure;
    }
    if (*pPara++ != ',') { // skip ','
        goto failure;
    }
    //获取第2个参数 value
    if(!at_get_next_int_dec(&pPara, &value)) {
        goto failure;
    }

    pin = pinNameToPin(pinName);
    if(pin < 0) {
        goto failure;
    }

    log_v("pinName = %s, pin = %d, value = %d\r\n", pinName, pin, value);
    digitalWrite(pin, value);
    at_response_ok();
    return;

failure:
    at_response_error();
}

void at_setupCmdRfCheck(uint8_t id, char *pPara)
{
    log_v("at_setupCmdRfCheck\r\n");
#if PLATFORM_ID == PLATFORM_ANT
    double freq = 0;
    char datarate[16] = {0};
    int tx_rssi, rx_rssi;
    String respString = "";

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 freq
    if(!at_get_next_float(&pPara, &freq)) {
        goto failure;
    }
    if (*pPara++ != ',') { // skip ','
        goto failure;
    }
    //获取第2个参数 datarate
    if(at_data_str_copy(datarate, &pPara, sizeof(datarate)) <= 0) {
        goto failure;
    }

    log_v("freq = %f, datarate = %s\r\n", freq, datarate);
    if(rfCheck(freq, datarate, tx_rssi, rx_rssi)) {
        respString = "+RFCHECK:";
        respString += tx_rssi;
        respString += ",";
        respString += rx_rssi;
        respString += "\r\n";
        at_response(respString.c_str());
        at_response_ok();
        return;
    }

failure:
    at_response_error();

#elif PLATFORM_ID == PLATFORM_ATOM || PLATFORM_ID == PLATFORM_FIG || PLATFORM_ID == PLATFORM_NEUTRON || PLATFORM_ID == PLATFORM_NUT
    char ssid[64] = {0};
    uint8_t value = 0;
    int found = 0, n = 0;
    String respString = "";
    WiFiAccessPoint ap[10];

    if (*pPara++ != '=') {// skip '='
        goto failure;
    }
    //获取第1个参数 ssid
    if(at_data_str_copy(ssid, &pPara, sizeof(ssid)) <= 0) {
        goto failure;
    }

    log_v("ssid = %s\r\n", ssid);

    found = WiFi.scan(ap, 10);
    if(found < 1) {
        goto failure;
    }

    for(n = 0; n < found; n++) {
        if(!strcmp(ap[n].ssid, ssid)) {
            break;
        }
    }

    if(n == found) {
        goto failure;
    }

    respString = "+RFCHECK:\"";
    respString += ap[n].ssid;
    respString += "\",";
    respString += ap[n].rssi;
    respString += "\r\n";
    at_response(respString.c_str());
    at_response_ok();
    return;

failure:
    at_response_error();

#else
    at_response_ok();
#endif
}

void at_exeCmdRfCheck(uint8_t id)
{
    log_v("at_exeCmdRfCheck\r\n");
#if PLATFORM_ID == PLATFORM_FOX
    String respString = "";

    CellularSignal sig = Cellular.RSSI();
    respString = "+RFCHECK:\"";
    respString += sig.rssi;
    respString += "\r\n";
    at_response(respString.c_str());
    at_response_ok();
#else
    at_response_ok();
#endif
}

const at_cmd_t at_cmd[at_cmdNum]={
    //基础AT指令
    {NULL,            0,  NULL, NULL, NULL, at_exeCmdNull},
    {"E",             1,  NULL, NULL, at_setupCmdE, NULL},
    {"+RST",          4,  NULL, NULL, NULL, at_exeCmdRst},
    {"+RESTORE",      8,  NULL, NULL, NULL, at_exeCmdRestore},
    {"+CONFIG",       7,  NULL, NULL, NULL, at_exeCmdConfig},
    //设置类AT指令
    {"+INFO",         5,  NULL, at_queryCmdInfo, NULL, NULL},
    {"+GETPINMODE",   11, NULL, NULL, at_setupCmdGetPinMode, NULL},
    {"+SETPINMODE",   11, NULL, NULL, at_setupCmdSetPinMode, NULL},
    {"+ANALOGREAD",   11, NULL, NULL, at_setupCmdAnalogRead, NULL},
    {"+DIGITALREAD",  12, NULL, NULL, at_setupCmdDigitalRead, NULL},
    {"+DIGITALWRITE", 13, NULL, NULL, at_setupCmdDigitalWrite, NULL},
    {"+RFCHECK",      8,  NULL, NULL, at_setupCmdRfCheck, at_exeCmdRfCheck}
};

