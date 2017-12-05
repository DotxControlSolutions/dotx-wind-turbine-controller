/*
 * debugger.h
 *
 *  Created on: Apr 13, 2015
 *      Author: Juan
 */

#ifndef SRC_SUPLIB_DEBUGGER_H_
#define SRC_SUPLIB_DEBUGGER_H_


#define DIDMAXSIZE 1024

// The function to be called is DotxDebug. The macro then converts this into the internal function that is used.

#ifdef DOTXDEBUGGER

#ifndef _MITA_EXPORT_

#include <stdio.h>
#include <stdarg.h>

#define DotxDebug(...) DotxInternalDebug(__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__) 
void DotxInternalDebug( const char *file, const char *function, const int line, const char *format, ...);

#else

#include <os_main.h>
#include <lib_main.h>
#define DotxDebug(...) OS_DEBUGLOG_make_entry(1234, __VA_ARGS__)

#endif
#else

// No debug, expand to nothing
#define DotxDebug(...)

#endif


#endif /* SRC_SUPLIB_DEBUGGER_H_ */
