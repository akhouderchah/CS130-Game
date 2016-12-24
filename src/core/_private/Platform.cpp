#include "Platform.h"

bool programInitialize()
{
	bool success = ~false;
#if(SYSTEM_TYPE == EP_WINDOWS)
#ifndef _DEBUG
	success &= FreeConsole();
#endif//_DEBUG

#elif(SYSTEM_TYPE == EP_LINUX)

#elif(SYSTEM_TYPE == EP_MAC_OSX)

#endif
	return success;
}

#ifdef _CUSTOM_BSWAP
uint16_t bswap16(uint16_t value)
{
	return ((value & 0xFF00) >> 8u) |
		((value & 0x00FF) << 8u);
}

uint32_t bswap32(uint32_t value)
{
	return ((value & 0xFF000000u) >> 24u) |
		((value & 0x00FF0000u) >> 8u) |
		((value & 0x0000FF00u) << 8u) |
		((value & 0x000000FFu) << 24u);
}

uint64_t bswap64(uint64_t value)
{
	return ((value & 0xFF00000000000000ull) >> 56u) |
		((value & 0x00FF000000000000ull) >> 40u) |
		((value & 0x0000FF0000000000ull) >> 24u) |
		((value & 0x000000FF00000000ull) >>  8u) |
		((value & 0x00000000FF000000ull) <<  8u) |
		((value & 0x0000000000FF0000ull) << 24u) |
		((value & 0x000000000000FF00ull) << 40u) |
		((value & 0x00000000000000FFull) << 56u);
}
#endif //_CUSTOM_BSWAP
