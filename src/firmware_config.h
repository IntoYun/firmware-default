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

#ifndef FIRMWARE_CONFIG_H_
#define FIRMWARE_CONFIG_H_

#include "Arduino.h"


#define FIRMWARE_COMM_STREAM                Serial
#define FIRMWARE_DEBUG_STREAM               Serial

// 选择用户日志打印级别后，将打印选择级别及高级别的日志信息
// 0 - FIRMWARE_LOG_LEVEL_NONE     :  无调试信息       最高级别
// 1 - FIRMWARE_LOG_LEVEL_ERROR    :  打印错误信息
// 2 - FIRMWARE_LOG_LEVEL_WARN     :  打印警告信息
// 3 - FIRMWARE_LOG_LEVEL_INFO     :  打印描述信息
// 4 - FIRMWARE_LOG_LEVEL_DEBUG    :  打印调试信息
// 5 - FIRMWARE_LOG_LEVEL_VERBOSE  :  打印详细信息     最低级别

#define FIRMWARE_DEBUG_LEVEL                 5

#endif

