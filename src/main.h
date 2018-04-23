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

#ifndef MAIN_H_
#define MAIN_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    char *pinName;
    uint16_t pin;
} pinmap_t;

#define PIN_MAP_NUM   41

extern const pinmap_t pinMap[PIN_MAP_NUM];

extern void userInit(void);
extern void userHandle(void);
extern int pinNameToPin(char *pinName);
extern bool rfCheck(double freq, char *datarate, int &tx_rssi, int &rx_rssi);

#ifdef __cplusplus
}
#endif

#endif

