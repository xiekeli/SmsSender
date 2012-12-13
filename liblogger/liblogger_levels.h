/*
 * liblogger - copyright 2007, Vineeth Neelakant, nvineeth@gmail.com
 * This file is part of liblogger.
 *
 * liblogger is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * liblogger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __EXP_LOGGER_CONFIG_H__
#define __EXP_LOGGER_CONFIG_H__

/** \defgroup GRP_LOG_LEVELS Log Levels
 * The following macros indicate the log levels
 * @{
 * */
/** The Trace log level */
#define LOG_LEVEL_TRACE	1
/** The Debug log level */
#define LOG_LEVEL_DEBUG 2
/** The Info log level */
#define LOG_LEVEL_INFO	3
/** The Warning log level */
#define LOG_LEVEL_WARN	4
/** The Error log level */
#define LOG_LEVEL_ERROR	5
/** The Fatal log level */
#define LOG_LEVEL_FATAL	6

/** define this macro to disable logs (in the current file), to disable globally define DISABLE_ALL_LOGS   */
#define LOG_LEVEL_DISABLED 12

/** Indicates the log level */
typedef enum LogLevel
{
	Trace, /**< The trace log level */
	Debug, /**< The debug log level */
	Info, /**< The info log level */
	Warn, /**< The warn log level */
	Error, /**< The error log level */
	Fatal /**< The fatal log level */
}LogLevel;

/** @} */

#endif // __EXP_LOGGER_CONFIG_H__

