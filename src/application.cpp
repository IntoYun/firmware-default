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

#include "application-nut.h"
#include "application-atom.h"
#include "application-fig.h"
#include "application-fox.h"
#include "application-neutron.h"
#include "application-nut.h"

void setup ()
{
    userInit();
}

void loop ()
{
    //loop 尽量不要阻塞
    userHandle();
}

#endif

