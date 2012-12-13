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
#ifndef __LIBLOGGER_CONFIG_H__
#define __LIBLOGGER_CONFIG_H__

/** define this macro to disable all logs */
//#define DISABLE_ALL_LOGS

/** Define this macro to prevent checking the variadic macro support of your compiler.
 * By defining this macro, you can also stop the filenames, function name, and the line no
 * from appearing in the logs.
 * */
//#define DISABLE_VARIADIC_CHECK

/** Define this macro to forcefully enable the varadic macro support. This can lead to compile time
 * errors if the compiler doesnot support variadic macros.*/
//#define ASSUME_VARIADIC_SUPPORT

/** Define this macro to prevent filenames from appearing in the logs. This is useful if the filenames are very long
 * and their presence is not desired in the logs.
 * */
//#define DISABLE_FILENAMES

#endif // __LIBLOGGER_CONFIG_H__
