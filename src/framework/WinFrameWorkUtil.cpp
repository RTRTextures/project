#include <Windows.h>

namespace Framework
{
    unsigned long long SystemMicrosecsTimestamp()
    {
        FILETIME systemTimeAsFileTime;
        GetSystemTimeAsFileTime(&systemTimeAsFileTime);

        ULARGE_INTEGER li;
        li.LowPart = systemTimeAsFileTime.dwLowDateTime;
        li.HighPart = systemTimeAsFileTime.dwHighDateTime;

        return li.QuadPart / 10;
    }
}