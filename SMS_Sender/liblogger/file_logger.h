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
#ifndef __FILE_LOGGER_H__
#define __FILE_LOGGER_H__

#include <stdio.h>

/* The rollback feature has been disabled, due to limitations 
 * in open modes of fopen(), will be enabled after further study.
 * */
#undef _ENABLE_LL_ROLLBACK_

/** File open mode.
 * */
typedef enum tFileOpenMode
{
	/** Indicates that the log file should be opened in append mode. */
	AppendMode = 0x2,
#ifdef _ENABLE_LL_ROLLBACK_
	/** Rollback Mode : The log file will be opened in append mode and will
	 * roll back when the size indicated 
	 * by \ref tFileLoggerInitParams::rollbackSize is reached.
	 * */
	RollbackMode
#endif // _ENABLE_LL_ROLLBACK_
} tFileOpenMode;

/** File Logger Initialization parameters. */
typedef struct tFileLoggerInitParams
{
	/** The filename of the log. */
	char* 			fileName;
	/** The file open mode. */
	tFileOpenMode 	fileOpenMode;
#ifdef _ENABLE_LL_ROLLBACK_
	/** The rollback size in \b bytes to use, when \ref tFileLoggerInitParams::fileOpenMode "fileOpenMode"
	 * is equal \ref tFileOpenMode::RollbackMode "RollbackMode"
	 * */
	unsigned long	rollbackSize;
#endif // _ENABLE_LL_ROLLBACK_
}tFileLoggerInitParams;

#endif // __FILE_LOGGER_H__
