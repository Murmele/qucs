/***************************************************************************
                                platform.h
                                ----------
    begin                : yes
    copyright            : 2016 by Felix Salfelder
    email                : felix@salfelder.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// platform dependent definitions
// inspired by gnucap md.h

#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED
/*--------------------------------------------------------------------------*/
/* autoconf stuff */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#if 0 // not yet.
#define	ENDDIR		"/"
#define PATHSEP		':'
#define DEFAULT_PLUGINS "qucs-default-plugins.so"
#define SYSTEMSTARTFILE	"gnucap.rc"
#define SYSTEMSTARTPATH	OS::getenv("QUCS_STARTPATH")
#define USERSTARTFILE	".gnucaprc"
#define	USERSTARTPATH	OS::getenv("HOME")
#define SHELL		OS::getenv("SHELL")
#endif
/*--------------------------------------------------------------------------*/
/* machine and compiler patches */
#if defined(__MINGW32__)
  // #define SIGSETJMP_IS_BROKEN not yet required.
  #define MS_DLL
#endif
/*--------------------------------------------------------------------------*/
// dynamic cast kludge.
// Strictly, this should always be dynamic_cast, but if it has already
// been checked, don't bother checking again, hence static_cast.
// It works and is faster.
#if defined(NDEBUG)
  #define prechecked_cast static_cast
#else
  #define prechecked_cast dynamic_cast
#endif

/*--------------------------------------------------------------------------*/
/* portability hacks, the serious stuff */

#if !defined(MS_DLL)
  // The usual way for POSIX compliant systems
  #include <dlfcn.h>
  #define INTERFACE
#else
// Microsoft DLL hacks -- thanks to Holger Vogt and Cesar Strauss for the info
// Make the MS DLL functions look like the posix ones.
#include <windows.h>
#undef min
#undef max
#undef INTERFACE
  #ifdef MAKE_DLL
    #define INTERFACE __declspec(dllimport)
  #else
    #define INTERFACE __declspec(dllexport)
  #endif

inline void* dlopen(const char* f, int)
{
  return LoadLibrary(const_cast<char*>(f));
}

inline void dlclose(void* h)
{
  FreeLibrary((HINSTANCE)h);
}

inline char* dlerror()
{
  static LPVOID lpMsgBuf = NULL;
  // free the error message buffer
  if (lpMsgBuf) {
    LocalFree(lpMsgBuf);
  }
  // get the error code
  DWORD dw = GetLastError();
  // get the corresponding error message
  FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL);
  return (char*)lpMsgBuf;
}
#define RTLD_LAZY       0x00001 /* Lazy function call binding.  */
#define RTLD_NOW        0x00002 /* Immediate function call binding.  */
#define RTLD_BINDING_MASK   0x3 /* Mask of binding time value.  */
#define RTLD_NOLOAD     0x00004 /* Do not load the object.  */
#define RTLD_DEEPBIND   0x00008 /* Use deep binding.  */
#define RTLD_GLOBAL     0x00100
#define RTLD_LOCAL      0
#define RTLD_NODELETE   0x01000
#endif

#if defined(SIGSETJMP_IS_BROKEN)
  #undef sigjmp_buf
  #undef siglongjmp
  #undef sigsetjmp
  #define sigjmp_buf jmp_buf
  #define siglongjmp(a,b) longjmp(a,b)
  #define sigsetjmp(a,b) setjmp(a)
#endif

#if !defined(SIGNALARGS)
  #define SIGNALARGS int
#endif
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
// vim:ts=8:sw=2:noet: