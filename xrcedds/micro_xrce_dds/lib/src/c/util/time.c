#include <uxr/client/util/time.h>
#include <uxr/client/config.h>

#if defined(PLATFORM_NAME_LINUX) || defined(PLATFORM_NAME_WINDOWS)

//==================================================================
//                             PUBLIC
//==================================================================
int64_t uxr_millis(void)
{
#ifdef WIN32
    SYSTEMTIME epoch_tm = {1970, 1, 4, 1, 0, 0, 0, 0};
    FILETIME epoch_ft;
    SystemTimeToFileTime(&epoch_tm, &epoch_ft);
    uint64_t epoch_time = (((uint64_t) epoch_ft.dwHighDateTime) << 32) + epoch_ft.dwLowDateTime;

    SYSTEMTIME tm;
    FILETIME ft;
    GetSystemTime(&tm);
    SystemTimeToFileTime(&tm, &ft);
    uint64_t current_time = (((uint64_t) ft.dwHighDateTime) << 32) + ft.dwLowDateTime;

    return (current_time - epoch_time) / 10000;
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (((int64_t)ts.tv_sec) * 1000) + (ts.tv_nsec / 1000000);
#endif
}

#endif