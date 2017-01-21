#pragma once

#include "Config.h"
#include <cstdint>

enum EPlatforms
{
	EP_INVALID = 0,
	EP_LINUX = 1,
	EP_WINDOWS = 2,
	EP_MAC_OSX = 3
};

enum EBuildTypes
{
	EBT_DEBUG = 0,
	EBT_RELEASE
};

#if(BUILD_TYPE == EBT_DEBUG)
#undef _DEBUG
#define _DEBUG
#else
#undef _DEBUG
#endif

#if(SYSTEM_TYPE == EP_WINDOWS)
#include <stdlib.h>
#elif(SYSTEM_TYPE == EP_LINUX)
#include <endian.h>
#elif(SYSTEM_TYPE == EP_MAC_OSX)
// OSX-specific includes
#else
// Platform-indendent alternatives only
#endif

#if COMPILER_IS_GCC
#define __method__ __PRETTY_FUNCTION__
#elif COMPILER_IS_MSVC
#define __method__ __FUNCTION__
#else
#define __method__ __func__
#endif

// Byte-swap for handling endianness issues
#if COMPILER_IS_GCC && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 3) || __GNUC__ >  4)
#define bswap16 __builtin_bswap16
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#elif COMPILER_IS_MSVC
#define bswap16 _byteswap_ushort
#define bswap32 _byteswap_ulong
#define bswap64 _byteswap_uint64
#else
#define _CUSTOM_BSWAP
uint16_t bswap16(uint16_t value);
uint32_t bswap32(uint32_t value);
uint64_t bswap64(uint64_t value);
#endif

#define bxchg16(x) ((x) = bswap16(x))
#define bxchg32(x) ((x) = bswap32(x))
#define bxchg64(x) ((x) = bswap64(x))

// Run at the beginning of execution to do platform-specific initialization
bool programInitialize();

