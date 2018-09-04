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

#include "Arduino.h"
#include "firmware_at.h"
#include "application_config.h"
#include "main.h"

PRODUCT_ID(PRODUCT_ID_DEF)                        // 产品标识
PRODUCT_SECRET(PRODUCT_SECRET_DEF)                // 产品密钥
PRODUCT_SOFTWARE_VERSION(SOFTWARE_VERSION_DEF)    // 产品软件版本号

int pinNameToPin(char *pinName)
{
    int count = sizeof(pinMap)/sizeof(pinmap_t);
    int pin = -1;

    for(int n = 0; n < count; n++) {
        if(!strcmp(pinName, pinMap[n].pinName)) {
            pin = pinMap[n].pin;
            break;
        }
    }
    return pin;
}

void init_before_setup(void)
{
    //Log.setLevel("*", MOLMC_LOG_VERBOSE);
}

STARTUP( init_before_setup() );

void setup ()
{
    at_init();
    userInit();
}

void loop ()
{
    //loop 尽量不要阻塞
    userHandle();
    at_loop();
}

