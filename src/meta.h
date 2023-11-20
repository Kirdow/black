#pragma once

#define BLACK_NAME "black"
#define BLACK_VERSION "0.1.0"
#define BLACK_LICENSE "MIT License"
#define BLACK_LICENSE_URL "https://kirdow.github.io/black/LICENSE.txt"
#define BLACK_LICENSE_TEXT0 "Licensed under " BLACK_LICENSE "."
#define BLACK_LICENSE_TEXT1 "Visit " BLACK_LICENSE_URL " for the full license."
#define BLACK_COPY_YEAR "2023"
#define BLACK_COPY_NAME "Dennis Rönn"

#ifdef _WIN32
	#define BLACK_PLATFORM "Windows"
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define BLACK_PLATFORM "iPhone Simulator"
	#elif TARGET_OS_IPHONE == 1
		#define BLACK_PLATFORM "iOS"
	#elif TARGET_OS_MAC == 1
		#define BLACK_PLATFORM "macOS"
	#else
		#define BLACK_PLATFORM "Unknown Apple Platform"
	#endif
#elif defined(__ANDROID__)
	#define BLACK_PLATFORM "Android"
#elif defined(__linux__)
	#define BLACK_PLATFORM "Linux"
#else
	#define BLACK_PLATFORM "Unknown Platform"
#endif
