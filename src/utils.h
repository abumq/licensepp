//
//  utils.h
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef Utils_h
#define Utils_h

#if (defined(_WIN32) || defined(_WIN64))
#  define LICENSEPP_OS_WINDOWS 1
#else
#  define LICENSEPP_OS_WINDOWS 0
#endif
#if (defined(__linux) || defined(__linux__))
#  define LICENSEPP_OS_LINUX 1
#else
#  define LICENSEPP_OS_LINUX 0
#endif
#if (defined(__APPLE__))
#  define LICENSEPP_OS_MAC 1
#else
#  define LICENSEPP_OS_MAC 0
#endif
#if (defined(__FreeBSD__) || defined(__FreeBSD_kernel__))
#  define LICENSEPP_OS_FREEBSD 1
#else
#  define LICENSEPP_OS_FREEBSD 0
#endif
#if (defined(__sun))
#  define LICENSEPP_OS_SOLARIS 1
#else
#  define LICENSEPP_OS_SOLARIS 0
#endif
#if (defined(_AIX))
#  define LICENSEPP_OS_AIX 1
#else
#  define LICENSEPP_OS_AIX 0
#endif
#if (defined(__NetBSD__))
#  define LICENSEPP_OS_NETBSD 1
#else
#  define LICENSEPP_OS_NETBSD 0
#endif
#if ((LICENSEPP_OS_LINUX || LICENSEPP_OS_MAC || LICENSEPP_OS_FREEBSD || LICENSEPP_OS_NETBSD || LICENSEPP_OS_SOLARIS || LICENSEPP_OS_AIX) && (!LICENSEPP_OS_WINDOWS))
#  define LICENSEPP_OS_UNIX 1
#else
#  define LICENSEPP_OS_UNIX 0
#endif

#if LICENSEPP_OS_UNIX
#   include <sys/time.h>
#endif
#include <ctime>
#include <chrono>
#include <string>

#if LICENSEPP_OS_WINDOWS
struct timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
};
#endif

namespace licensepp {

///
/// \brief Contains static utility functions used by Residue
///
class Utils
{
public:
    static const char* kDays[7];
    static const char* kDaysAbbrev[7];
    static const char* kMonths[12];
    static const char* kMonthsAbbrev[12];

    static inline uint64_t now()
    {
        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
    }

    static uint64_t nowUtc();

    static std::string timevalToString(struct timeval tval, const char* format);
    static struct ::tm* buildTimeInfo(struct timeval* currTime, struct ::tm* timeInfo);
    static char* convertAndAddToBuff(std::size_t n, int len, char* buf, const char* bufLim, bool zeroPadded = true);

    static char* addToBuff(const char* str, char* buf, const char* bufLim);

    static char* parseFormat(char* buf, std::size_t bufSz, const char* format, const struct tm* tInfo, std::size_t msec);
};
}
#endif /* Utils_h */
