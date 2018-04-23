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
        at_response_error();
        return;
    }
    at_response_ok();
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
    at_response_ok();
}

void at_setupCmdGetPinMode(uint8_t id, char *pPara)
{
    log_v("at_setupCmdGetPinMode\r\n");
    at_response_ok();
}

void at_setupCmdSetPinMode(uint8_t id, char *pPara)
{
    log_v("at_setupCmdSetPinMode\r\n");
    at_response_ok();
}

void at_setupCmdAnalogRead(uint8_t id, char *pPara)
{
    log_v("at_setupCmdAnalogRead\r\n");
    at_response_ok();
}

void at_setupCmdDigitalRead(uint8_t id, char *pPara)
{
    log_v("at_setupCmdDigitalRead\r\n");
    at_response_ok();
}

void at_setupCmdDigitalWrite(uint8_t id, char *pPara)
{
    log_v("at_setupCmdDigitalWrite\r\n");
    at_response_ok();
}

void at_setupCmdRfCheck(uint8_t id, char *pPara)
{
    log_v("at_setupCmdRfCheck\r\n");
    at_response_ok();
}

at_cmd_t at_cmd[at_cmdNum]={
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
    {"+RFCHECK",      8,  NULL, NULL, at_setupCmdRfCheck, NULL},
};

