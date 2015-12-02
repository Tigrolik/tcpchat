/*
 * common header file to be used in other files
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * platforms: might be used in future if it would be necessary to make the
 * code portable. For now it is intended to be used in UNIX env
 */
#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC	 2
#define PLATFORM_UNIX	 3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
// trick for windows, the lazy way
#pragma comment(lib, "wsock32.lib")
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
// include basic libraries
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#endif
#endif
