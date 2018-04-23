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

#ifndef APPLICATION_CONFIG_H__
#define APPLICATION_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if PLATFORM_ID == PLATFORM_ANT
#define PRODUCT_ID_DEF                            ddab5c4700000359
#define PRODUCT_SECRET_DEF                        c93bbb81ec731fbbaea5625fe12271e1
#define SOFTWARE_VERSION_DEF                      1.0.0
#elif PLATFORM_ID == PLATFORM_ATOM
#define PRODUCT_ID_DEF                            1ef8893400000355
#define PRODUCT_SECRET_DEF                        46a4fa8496c8521185b9e4c824c9c08c
#define SOFTWARE_VERSION_DEF                      1.0.0
#elif PLATFORM_ID == PLATFORM_FIG
#define PRODUCT_ID_DEF                            babd230f00000353
#define PRODUCT_SECRET_DEF                        40a46ad251e7546c2eb9d008ad8d845f
#define SOFTWARE_VERSION_DEF                      1.0.0
#elif PLATFORM_ID == PLATFORM_FOX
#define PRODUCT_ID_DEF                            6a07526400000358
#define PRODUCT_SECRET_DEF                        f7532c9a674ec9a47914a02a4a0c5f69
#define SOFTWARE_VERSION_DEF                      1.0.0
#elif PLATFORM_ID == PLATFORM_NEUTRON
#define PRODUCT_ID_DEF                            a8a89f0c00000354
#define PRODUCT_SECRET_DEF                        d0131a24abd29adcfb1c3748bba76128
#define SOFTWARE_VERSION_DEF                      1.0.0
#elif PLATFORM_ID == PLATFORM_NUT
#define PRODUCT_ID_DEF                            05595c4000000352
#define PRODUCT_SECRET_DEF                        603cb77eb46832ef76834bcb669e88bf
#define SOFTWARE_VERSION_DEF                      1.0.0
#endif

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_CONFIG_H__*/

