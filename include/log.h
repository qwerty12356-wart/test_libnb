#include "android/log.h"

#ifdef LOG_DEBUG
#define debug_print(...) __android_log_print(ANDROID_LOG_DEBUG, "libnb_custom", __VA_ARGS__)
#else
#define debug_print(...)
#endif
#define error_print(...) __android_log_print(ANDROID_LOG_ERROR, "libnb_custom", __VA_ARGS__)
#define info_print(...) __android_log_print(ANDROID_LOG_INFO, "libnb_custom", __VA_ARGS__)