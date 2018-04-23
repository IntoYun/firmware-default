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

#ifndef FIRMWARE_LOG_H__
#define FIRMWARE_LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    DEBUG_LEVEL_NONE = 0,
    DEBUG_LEVEL_ERROR,
    DEBUG_LEVEL_WARN,
    DEBUG_LEVEL_INFO,
    DEBUG_LEVEL_DEBUG,
    DEBUG_LEVEL_VERBOSE
} debug_level_t;

#ifndef CONFIG_FIRMWARE_LOG_COLORS
#define CONFIG_FIRMWARE_LOG_COLORS 0
#endif

#if CONFIG_FIRMWARE_LOG_COLORS
#define FIRMWARE_LOG_COLOR_BLACK   "30"
#define FIRMWARE_LOG_COLOR_RED     "31" //ERROR
#define FIRMWARE_LOG_COLOR_GREEN   "32" //INFO
#define FIRMWARE_LOG_COLOR_YELLOW  "33" //WARNING
#define FIRMWARE_LOG_COLOR_BLUE    "34"
#define FIRMWARE_LOG_COLOR_MAGENTA "35"
#define FIRMWARE_LOG_COLOR_CYAN    "36" //DEBUG
#define FIRMWARE_LOG_COLOR_GRAY    "37" //VERBOSE
#define FIRMWARE_LOG_COLOR_WHITE   "38"

#define FIRMWARE_LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define FIRMWARE_LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define FIRMWARE_LOG_RESET_COLOR   "\033[0m"

#define FIRMWARE_LOG_COLOR_E       FIRMWARE_LOG_COLOR(FIRMWARE_LOG_COLOR_RED)
#define FIRMWARE_LOG_COLOR_W       FIRMWARE_LOG_COLOR(FIRMWARE_LOG_COLOR_YELLOW)
#define FIRMWARE_LOG_COLOR_I       FIRMWARE_LOG_COLOR(FIRMWARE_LOG_COLOR_GREEN)
#define FIRMWARE_LOG_COLOR_D       FIRMWARE_LOG_COLOR(FIRMWARE_LOG_COLOR_CYAN)
#define FIRMWARE_LOG_COLOR_V       FIRMWARE_LOG_COLOR(FIRMWARE_LOG_COLOR_GRAY)
#else
#define FIRMWARE_LOG_COLOR_E
#define FIRMWARE_LOG_COLOR_W
#define FIRMWARE_LOG_COLOR_I
#define FIRMWARE_LOG_COLOR_D
#define FIRMWARE_LOG_COLOR_V
#define FIRMWARE_LOG_RESET_COLOR
#endif

const char * pathToFileName(const char * path);
void log_firmware_print(debug_level_t level, const char *fmt, ...);
void log_firmware_print_dump(debug_level_t level, uint8_t *buf, uint16_t len);

#define FIRMWARE_SHORT_LOG_FORMAT(letter, format)  FIRMWARE_LOG_COLOR_ ## letter format FIRMWARE_LOG_RESET_COLOR ""
#define FIRMWARE_LOG_FORMAT(letter, format)  FIRMWARE_LOG_COLOR_ ## letter "[" #letter "][%20s:%u] %15s() --> " format FIRMWARE_LOG_RESET_COLOR "", pathToFileName(__FILE__), __LINE__, __FUNCTION__

#define log_v(format, ...) log_firmware_print(DEBUG_LEVEL_VERBOSE, FIRMWARE_SHORT_LOG_FORMAT(V, format), ##__VA_ARGS__)
#define log_v_dump(data, len) log_firmware_print_dump(DEBUG_LEVEL_VERBOSE, data, len)

#define log_d(format, ...) log_firmware_print(DEBUG_LEVEL_DEBUG, FIRMWARE_SHORT_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define log_d_dump(data, len) log_firmware_print_dump(DEBUG_LEVEL_DEBUG, data, len)

#define log_i(format, ...) log_firmware_print(DEBUG_LEVEL_INFO, FIRMWARE_SHORT_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define log_i_dump(data, len) log_firmware_print_dump(DEBUG_LEVEL_INFO, data, len)

#define log_w(format, ...) log_firmware_print(DEBUG_LEVEL_WARN, FIRMWARE_SHORT_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define log_w_dump(data, len) log_firmware_print_dump(DEBUG_LEVEL_WARN, data, len)

#define log_e(format, ...) log_firmware_print(DEBUG_LEVEL_ERROR, FIRMWARE_SHORT_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define log_e_dump(data, len) log_firmware_print_dump(DEBUG_LEVEL_ERROR, data, len)

#if !defined(CONFIG_NOASSERT)
#define ASSERT(cond) if(!(cond)) log_firmware_failed(__FILE__, __LINE__)
#else
#define ASSERT(cond) /**/
#endif

void log_firmware_int(void);
void log_firmware_set_level(uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* FIRMWARE_LOG_H__ */

