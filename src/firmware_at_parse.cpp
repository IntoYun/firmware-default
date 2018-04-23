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
#include "firmware_at_parse.h"
#include "firmware_at_cmd.h"
#include "firmware_log.h"


at_stateType at_state = at_statIdle;
bool echoFlag = true;
uint8_t at_cmdLine[at_cmdLenMax];

static int16_t at_cmdSearch(int8_t cmdLen, uint8_t *pCmd)
{
    int16_t i;

    if(cmdLen == 0) {
        return 0;
    } else if(cmdLen > 0) {
        for(i=1; i<at_cmdNum; i++)
        {
            if(cmdLen == at_cmd[i].at_cmdLen) {
                if(memcmp(pCmd, at_cmd[i].at_cmdName, cmdLen) == 0) {
                    return i;
                }
            }
        }
    }
    return -1;
}

static int8_t at_getCmdLen(uint8_t *pCmd)
{
    uint8_t n,i;

    n = 0;
    i = 128;

    while(i--)
    {
        if((*pCmd == '\r') || (*pCmd == '=') || (*pCmd == '?') || ((*pCmd >= '0')&&(*pCmd <= '9'))) {
            return n;
        } else {
            pCmd++;
            n++;
        }
    }
    return -1;
}

static void at_cmdProcess(uint8_t *pAtRcvData)
{
    char tempStr[32];

    int16_t cmdId;
    int8_t cmdLen;
    uint16_t i;

    cmdLen = at_getCmdLen(pAtRcvData);
    if(cmdLen != -1) {
        cmdId = at_cmdSearch(cmdLen, pAtRcvData);
    } else {
        cmdId = -1;
    }
    if(cmdId != -1) {
        pAtRcvData += cmdLen;
        if(*pAtRcvData == '\r') {
            if(at_cmd[cmdId].at_exeCmd) {
                at_cmd[cmdId].at_exeCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if(*pAtRcvData == '?' && (pAtRcvData[1] == '\r'))
        {
            if(at_cmd[cmdId].at_queryCmd) {
                at_cmd[cmdId].at_queryCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if((*pAtRcvData == '=') && (pAtRcvData[1] == '?') && (pAtRcvData[2] == '\r'))
        {
            if(at_cmd[cmdId].at_testCmd) {
                at_cmd[cmdId].at_testCmd(cmdId);
            } else {
                at_response_error();
            }
        }
        else if((*pAtRcvData >= '0') && (*pAtRcvData <= '9') || (*pAtRcvData == '='))
        {
            if(at_cmd[cmdId].at_setupCmd) {
                at_cmd[cmdId].at_setupCmd(cmdId, pAtRcvData);
            } else {
                at_response_error();
            }
        } else {
            at_response_error();
        }
    } else {
        at_response_error();
    }
}

//接收到的数据与字符串作对比
static bool checkReadString(uint8_t *buffer, uint16_t bufferLen, uint8_t value, char *string)
{
    for(int n = 1; n < bufferLen; n++ ) {
        buffer[n-1] = buffer[n];
    }
    buffer[bufferLen-1] = value;

    if((memcmp(buffer, string, strlen(string)) == 0)) {
        return true;
    }
    return false;
}

void at_parse(void)
{
    static uint8_t atHead[6] = {0};
    static uint8_t *pCmdLine = NULL;
    static uint8_t *pDataLine = NULL;
    uint8_t temp;

    while(FIRMWARE_COMM_STREAM.available())
    {
        temp = FIRMWARE_COMM_STREAM.read();

        //处理回显
        if((temp != '\n') && (echoFlag)) {
            FIRMWARE_COMM_STREAM.print((char)temp);
        }

        switch(at_state) {
            case at_statIdle: //serch "AT" head
                //log_v("at_statIdle\r\n");
                if(checkReadString(atHead, 2, temp, "AT")) {
                    at_state = at_statRecving;
                    pCmdLine = at_cmdLine;
                    atHead[1] = 0x00;
                } else if(temp == '\n') {
                    at_response_error();
                }
                break;

            case at_statRecving: //push receive data to cmd line
                //log_v("at_statRecving\r\n");
                *pCmdLine = temp;
                if(temp == '\n') {
                    pCmdLine++;
                    *pCmdLine = '\0';
                    // deal at command
                    at_cmdProcess(at_cmdLine);
                    at_state = at_statIdle;
                } else if(pCmdLine >= &at_cmdLine[at_cmdLenMax - 1]) {
                    at_state = at_statIdle;
                }
                pCmdLine++;
                break;

            default:
                if(temp == '\n')
                {
                }
                break;
        }
    }
}

/**
  * @说明  从AT指令截取整数。处理完毕AT指令字符串指针跳至‘,’或者‘\n’。
  * @参数  p_src   : AT指令字符串
  *        result  : 整数指针
  * @返回  TRUE: 获取成功  FLASE: 获取失败
  */
bool at_get_next_int_dec(char **p_src, int *result)
{
    char temp[17] = {0}, value = 0;
    int pos = 0;
    uint16_t len = 0;
    bool flag = true;

    len = strlen(*p_src);
    for(pos = 0; pos < len; pos++) {
        value = (*p_src)[pos];
        if((value == ',') || (value == '\r') || (value == '\n')) {break;}
        if(((value < '0') || (value > '9')) && (value != '-') && (value != ' ')) {
            flag = false;
        }
    }

    if(flag) {
        memset(temp, 0, sizeof(temp));
        if((pos+1) < sizeof(temp)) {
            memcpy(temp, *p_src, pos+1);
        } else {
            memcpy(temp, *p_src, sizeof(temp)-1);
        }
        *result = atoi(temp);
    }
    *p_src += pos;
    return flag ? true : false;
}

/**
  * @说明  从AT指令截取浮点数。处理完毕AT指令字符串指针跳至‘,’或者‘\n’。
  * @参数  p_src   : AT指令字符串
  *        result  : 浮点数指针
  * @返回  TRUE: 获取成功  FLASE: 获取失败
  */
bool at_get_next_float(char **p_src, double *result)
{
    char temp[17] = {0}, value = 0;
    int pos = 0;
    uint16_t len = 0;
    bool flag = true;

    len = strlen(*p_src);
    for(pos = 0; pos < len; pos++) {
        value = (*p_src)[pos];
        if((value == ',') || (value == '\r') || (value == '\n')) {break;}
        if(((value < '0') || (value > '9')) && (value != '-') && (value != ' ') && (value != '.')) {
            flag = false;
        }
    }

    if(flag) {
        memset(temp, 0, sizeof(temp));
        if((pos+1) < sizeof(temp)) {
            memcpy(temp, *p_src, pos+1);
        } else {
            memcpy(temp, *p_src, sizeof(temp)-1);
        }
        *result = atof(temp);
    }
    *p_src += pos;
    return flag ? true : false;
}

/**
  * @说明  从AT指令截取字符串。字符串包含在双引号之间。处理完毕AT指令字符串指针跳至‘,’或者‘\n’。
  * @参数  p_dest  : 字符串缓冲区
  *        p_src   : AT指令字符串
  *        max_len : 字符串缓冲区大小
  * @返回  获取字符串长度
  */
int32_t at_data_str_copy(char *p_dest, char **p_src, int32_t max_len)
{
    uint16_t len = 0;
    int i = 0, pos = 0, quote = 0, n = 0;
    char value = 0, start = false;

    len = strlen(*p_src);
    for(pos = 0, quote = 0; pos < len; pos++) {
        value = (*p_src)[pos];
        if(value == '\"') { quote++; }
        if((value == ',') || (value == '\r') || (value == '\n')) {break;}
    }
    if(quote >= 2) {  //存在字符串
        for(i = 0, n = 0; (n < (max_len-1))||(n <= pos); i++) {
            value = (*p_src)[i];
            if(value == '\"') {
                if(!start) {
                    start = true;
                    continue;
                } else {
                    break;
                }
            }
            if(start) {
                p_dest[n++] = (*p_src)[i];
            }
        }
    } else {
        n = 0;
    }
    p_dest[n] = 0;
    *p_src += pos;
    return n;
}

/**
  * @brief  Response "OK" to uart.
  * @param  None
  * @retval None
  */
void at_response_ok(void)
{
    FIRMWARE_COMM_STREAM.print("\r\nOK\r\n");
}

/**
  * @brief  Response "ERROR" to uart.
  * @param  None
  * @retval None
  */
void at_response_error(void)
{
    FIRMWARE_COMM_STREAM.print("\r\nERROR\r\n");
}

/**
  * @brief  Response string.
  *  It is equivalent to at_port_print,if not call at_register_response_func or call at_register_response_func(NULL);
  *  It will run custom response function,if call at_register_response_func and parameter is not NULL.
  * @param  string
  * @retval None
  */
void at_response(const char *str)
{
    FIRMWARE_COMM_STREAM.print(str);
}

void at_response_data(const uint8_t *data, uint16_t dataLen)
{
    if(NULL == data) {
        return;
    }

    for(int i = 0; i < dataLen; i++) {
        FIRMWARE_COMM_STREAM.write(data[i]);
    }
}

