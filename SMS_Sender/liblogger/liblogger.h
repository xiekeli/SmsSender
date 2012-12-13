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
#ifndef __EXP_LOGGER_H__
#define __EXP_LOGGER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "liblogger_levels.h"
#include "liblogger_config.h"

/** Liblogger major version */
#define LIBLOGGER_MAJOR_VERSION 0
/** Liblogger minor version */
#define LIBLOGGER_MINOR_VERSION 1

/*If the default log level is not define,
 * then the default log level is set to Trace. */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

/* If the module name is not defined, then the default module name. */
#ifndef LOG_MODULE_NAME
#define LOG_MODULE_NAME ""
#endif



/** enum which indicates the log destination, used with \ref InitLogger */
typedef enum LogDest
{
	/** Indicates that logging should be done to file. */
	LogToFile,
	/** Log to Console. (stdout) */
	LogToConsole,
	/** Indicates that logging should be done to socket. Please note that log server should be 
	 *  running. */
	LogToSocket
}LogDest;

/* few compilers dont support variadic macros,so initially undef it, 
 * and depending on the compiler define it.
 */
#undef VARIADIC_MACROS

#if !defined(DISABLE_VARIADIC_CHECK) && (defined(__GNUC__) || (_MSC_VER >= 1400))
	/* 1. GCC supports variadic macros, 
	 * 2. this feature is available only in VS 2005 or higher versions.
	 * */
	#define VARIADIC_MACROS
#endif

/* Forcefully enable the variadic macro support, results in compiler errors if not supported. */
#ifdef ASSUME_VARIADIC_SUPPORT
	#define VARIADIC_MACROS
#endif	


#ifdef DISABLE_ALL_LOGS
#ifdef __GNUC__
// Logs needs to be disabled, warn the user.
#warning Logger disabled.
#endif // __GNUC__

	#define LogTrace		/* NOP */
	#define LogDebug 		/* NOP */
	#define LogInfo 		/* NOP */
	#define LogWarn			/* NOP */
	#define LogError		/* NOP */
	#define LogFatal		/* NOP */
	#define LogFuncEntry()	/* NOP */
	#define LogFuncExit()	/* NOP */
	#define InitLogger 		/* NOP */
	#define DeInitLogger()	/* NOP */
#else

	/* WIN32 support. */
	#if defined(WIN32) || defined(_WIN32)
	#define __func__ __FUNCTION__
	#endif

	// The logs are enabled.

	#ifdef VARIADIC_MACROS	
	int LogStub_vm(LogLevel logLevel,
		const char* moduleName,const char* file,
		const char* funcName, const int lineNum,
		const char* fmt,...);
	#endif
	/** 
	 * Function used to initialize the logger.
	 * \param [in] ldest The log destination. see \ref LogDest for possible destinations.
	 * \param [in] loggerInitParams The logger initialization parameters.
	 * \returns 0 if successful, -1 if there is a failure.
	 * */
	int InitLogger(LogDest ldest,void* loggerInitParams);

	/** Function used to deinitialize the logger. */
	void DeInitLogger();


	/* -- Log Level Trace -- */
	#if LOG_LEVEL<= LOG_LEVEL_TRACE
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogTrace(fmt, ...) LogStub_vm(Trace,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogTrace(fmt, ...) LogStub_vm(Trace,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Trace level. */
			int LogTrace(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogTrace	/*NOP*/
	#endif

	/* -- Log Level Debug -- */
	#if LOG_LEVEL<= LOG_LEVEL_DEBUG
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogDebug(fmt, ...) LogStub_vm(Debug,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogDebug(fmt, ...) LogStub_vm(Debug,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Debug level. */
			int LogDebug(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogDebug	/*NOP*/
	#endif

	/* -- Log Level Info -- */
	#if LOG_LEVEL<= LOG_LEVEL_INFO
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogInfo(fmt, ...) LogStub_vm(Info,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogInfo(fmt, ...) LogStub_vm(Info,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Info level. */
			int LogInfo(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogInfo /*NOP*/
	#endif

	/* -- Log Level Warn -- */
	#if LOG_LEVEL<= LOG_LEVEL_WARN
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogWarn(fmt, ...) LogStub_vm(Warn,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogWarn(fmt, ...) LogStub_vm(Warn,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Warn level. */
			int LogWarn(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogWarn	/*NOP*/
	#endif

	/* -- Log Level Error-- */
	#if LOG_LEVEL<= LOG_LEVEL_ERROR
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogError(fmt, ...) LogStub_vm(Error,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogError(fmt, ...) LogStub_vm(Error,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Error level. */
			int LogError(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogError	/*NOP*/
	#endif

	/* -- Log Level Fatal -- */
	#if LOG_LEVEL<= LOG_LEVEL_FATAL
		#ifdef VARIADIC_MACROS
			#if defined(DISABLE_FILENAMES)
				/* the filename should be disabled. */
				#define LogFatal(fmt, ...) LogStub_vm(Fatal,LOG_MODULE_NAME,"",__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#else 
				#define LogFatal(fmt, ...) LogStub_vm(Fatal,LOG_MODULE_NAME,__FILE__,__func__, __LINE__ , fmt , ## __VA_ARGS__)
			#endif // DISABLE_FILENAMES
		#else
			/** Emit a log with Fatal level. */
			int LogFatal(const char *fmt, ...);
		#endif // VARIADIC_MACROS
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogFatal	/*NOP*/
	#endif

	#if LOG_LEVEL<= LOG_LEVEL_TRACE
		/* Log Entry to a function. */
		int FuncLogEntry(const char* funcName);
		/* Log return from a function. */
		int FuncLogExit(const char* funcName,const int lineNumber);
		
		#define LogFuncEntry()	FuncLogEntry(__func__)
		#define LogFuncExit()	FuncLogExit(__func__,__LINE__)
		
	#else
		/* The chosen log level is greater, so this log should be disabled. */
		#define LogFuncEntry()	/* NOP */
		#define LogFuncExit()	/* NOP */
	#endif

#endif // DISABLE_ALL_LOGS


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EXP_LOGGER_H__ */

