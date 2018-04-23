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
#include "firmware_config.h"
#include "firmware_log.h"

#define MAX_DEBUG_MESSAGE_LENGTH        512

debug_level_t debug_level; //调试信息等级

void log_firmware_int(void)
{
    FIRMWARE_DEBUG_STREAM.begin(115200);   // As fast as possible for bus
    log_firmware_set_level(FIRMWARE_DEBUG_LEVEL);
}

void log_firmware_set_level(uint8_t level)
{
    debug_level = level;
}

const char * pathToFileName(const char * path)
{
    size_t i = 0;
    size_t pos = 0;
    char * p = (char *)path;
    while(*p){
        i++;
        if(*p == '/' || *p == '\\') {
            pos = i;
        }
        p++;
    }
    return path+pos;
}

void log_firmware_print(debug_level_t level, const char *fmt, ...)
{
    if (debug_level >= level) {
        char _buffer[MAX_DEBUG_MESSAGE_LENGTH];
        va_list args;

        va_start(args, fmt);
        int trunc = snprintf(_buffer, arraySize(_buffer), "[%010u] ", millis());
        FIRMWARE_DEBUG_STREAM.print(_buffer);
        trunc = vsnprintf(_buffer, arraySize(_buffer), fmt, args);
        FIRMWARE_DEBUG_STREAM.print(_buffer);
        if (trunc > arraySize(_buffer)) {
            FIRMWARE_DEBUG_STREAM.print("...");
        }
        va_end(args);
    }
}

void log_firmware_print_dump(debug_level_t level, uint8_t *buf, uint16_t len)
{
    if (debug_level >= level) {
        if(len > 0) {
            for(int i = 0; i < len-1; i++)
            {
                FIRMWARE_DEBUG_STREAM.printf("%02x", buf[i]);
                FIRMWARE_DEBUG_STREAM.print(':');
            }
            FIRMWARE_DEBUG_STREAM.printf("%02x", buf[len-1]);
            FIRMWARE_DEBUG_STREAM.print("\r\n");
        }
    }
}

void log_firmware_failed(const char *file, uint16_t line) {
    log_e("Program failed in file: %s, line: %d\r\n", file, line);
    while(1) {
        //System.process();
        Cloud.process();
    }
}

