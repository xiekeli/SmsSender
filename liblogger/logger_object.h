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
#ifndef __LOGGER_OBJECT_H__
#define __LOGGER_OBJECT_H__
#include <stdarg.h>
#include <stdio.h>
#include "liblogger.h"

/* fwd declaration. */
struct LogWriter;
typedef int (*Log)(struct LogWriter* _this,const LogLevel logLevel,
#ifdef VARIADIC_MACROS
		const char* moduleName,
		const char* file,const char* funcName, const int lineNum, 
#endif
		const char* fmt,va_list ap);
typedef int (*LogFuncEntry)(struct LogWriter * _this,const char* funcName);
typedef int (*LogFuncExit)(struct LogWriter* _this,const char* funcName,int lineNum);
typedef int (*LoggerDeInit)(struct LogWriter* _this);

/** The log writer object */
typedef struct LogWriter
{
	/** Member function to log. */
	Log 			log;
	/** Member function to log the function entry. */
	LogFuncEntry 	logFuncEntry;
	/** Member function to log the function exit. */
	LogFuncExit		logFuncExit;
	/** Member function to deinitialize the log writer object, the log writer object will
	 * not be referenced after this call.*/
	LoggerDeInit	loggerDeInit;
}LogWriter;


#endif // __LOGGER_OBJECT_H__
