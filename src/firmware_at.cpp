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
#include "firmware_at.h"
#include "firmware_log.h"

void at_init(void)
{
    FIRMWARE_COMM_STREAM.begin(115200);
}

void at_loop(void)
{
    at_parse();
}

