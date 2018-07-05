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

#ifndef FIRMWARE_AT_CMD_H__
#define FIRMWARE_AT_CMD_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    char *at_cmdName;
    int8_t at_cmdLen;
    void (*at_testCmd)(uint8_t id);
    void (*at_queryCmd)(uint8_t id);
    void (*at_setupCmd)(uint8_t id, char *pPara);
    void (*at_exeCmd)(uint8_t id);
}at_cmd_t;

#define at_cmdNum   12

extern const at_cmd_t at_cmd[at_cmdNum];

#ifdef __cplusplus
}
#endif

#endif /* FIRMWARE_AT_CMD_H__*/

