//
//  utils.cc
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#include <ctime>
#include "src/utils.h"

using namespace licensepp;

const char* Utils::kDays[7] = { "Sunday", "Monday", "Tuesday",
                                "Wednesday", "Thursday", "Friday",
                                "Saturday" };
const char* Utils::kDaysAbbrev[7] = { "Sun", "Mon", "Tue",
                                      "Wed", "Thu", "Fri", "Sat" };
const char* Utils::kMonths[12] = { "January", "February", "March",
                                   "Apri", "May", "June", "July",
                                   "August", "September", "October",
                                   "November", "December" };
const char* Utils::kMonthsAbbrev[12] = { "Jan", "Feb", "Mar", "Apr",
                                         "May", "Jun", "Jul", "Aug",
                                         "Sep", "Oct", "Nov", "Dec" };

uint64_t Utils::nowUtc()
{
    std::time_t t = std::time(nullptr);
    std::tm* nowTm;
    nowTm = std::gmtime(&t);
    return nowTm != nullptr ? mktime(nowTm) : 0;
}

std::string Utils::timevalToString(struct timeval tval, const char* format)
{
  struct ::tm timeInfo;
  buildTimeInfo(&tval, &timeInfo);
  const int kBuffSize = 30;
  char buff_[kBuffSize] = "";
  parseFormat(buff_, kBuffSize, format, &timeInfo, static_cast<std::size_t>(tval.tv_usec / 1000 /* subsecond = 3 */));
  return std::string(buff_);
}

struct ::tm* Utils::buildTimeInfo(struct timeval* currTime, struct ::tm* timeInfo)
{
#if LICENSEPP_OS_UNIX
  time_t rawTime = currTime->tv_sec;
  ::gmtime_r(&rawTime, timeInfo);
  return timeInfo;
#else
#  ifdef _MSC_VER
  time_t t;
#    if defined(_USE_32BIT_TIME_T)
  _time32(&t);
#    else
  _time64(&t);
#    endif
  gmtime_s(timeInfo, &t);
  return timeInfo;
#  else
  // For any other compilers that don't have CRT warnings issue e.g, MinGW or TDM GCC- we use different method
  time_t rawTime = currTime->tv_sec;
  struct tm* tmInf = gmtime(&rawTime);
  *timeInfo = *tmInf;
  return timeInfo;
#  endif  // _MSC_VER
#endif  // LICENSEPP_OS_UNIX
}

char* Utils::convertAndAddToBuff(std::size_t n, int len, char* buf, const char* bufLim, bool zeroPadded)
{
    char localBuff[10] = "";
    char* p = localBuff + sizeof(localBuff) - 2;
    if (n > 0) {
        for (; n > 0 && p > localBuff && len > 0; n /= 10, --len) {
            *--p = static_cast<char>(n % 10 + '0');
        }
    } else {
        *--p = '0';
        --len;
    }
    while (zeroPadded && p > localBuff && len-- > 0) {
        *--p = static_cast<char>('0');
    }
    return addToBuff(p, buf, bufLim);
}

char* Utils::addToBuff(const char* str, char* buf, const char* bufLim)
{
    while ((buf < bufLim) && ((*buf = *str++) != '\0')) {
        ++buf;
    }
    return buf;
}

char* Utils::parseFormat(char* buf, std::size_t bufSz, const char* format, const struct tm* tInfo,
                            std::size_t msec)
{
    const char* bufLim = buf + bufSz;
    for (; *format; ++format) {
        if (*format == '%') {
            switch (*++format) {
            case '%':  // Escape
                break;
            case '\0':  // End
                --format;
                break;
            case 'd':  // Day
                buf = convertAndAddToBuff(tInfo->tm_mday, 2, buf, bufLim);
                continue;
            case 'a':  // Day of week (short)
                buf = addToBuff(kDaysAbbrev[tInfo->tm_wday], buf, bufLim);
                continue;
            case 'A':  // Day of week (long)
                buf = addToBuff(kDays[tInfo->tm_wday], buf, bufLim);
                continue;
            case 'M':  // month
                buf = convertAndAddToBuff(tInfo->tm_mon + 1, 2, buf, bufLim);
                continue;
            case 'b':  // month (short)
                buf = addToBuff(kMonthsAbbrev[tInfo->tm_mon], buf, bufLim);
                continue;
            case 'B':  // month (long)
                buf = addToBuff(kMonths[tInfo->tm_mon], buf, bufLim);
                continue;
            case 'y':  // year (two digits)
                buf = convertAndAddToBuff(tInfo->tm_year + 1900, 2, buf, bufLim);
                continue;
            case 'Y':  // year (four digits)
                buf = convertAndAddToBuff(tInfo->tm_year + 1900, 4, buf, bufLim);
                continue;
            case 'h':  // hour (12-hour)
                buf = convertAndAddToBuff(tInfo->tm_hour % 12, 2, buf, bufLim);
                continue;
            case 'H':  // hour (24-hour)
                buf = convertAndAddToBuff(tInfo->tm_hour, 2, buf, bufLim);
                continue;
            case 'm':  // minute
                buf = convertAndAddToBuff(tInfo->tm_min, 2, buf, bufLim);
                continue;
            case 's':  // second
                buf = convertAndAddToBuff(tInfo->tm_sec, 2, buf, bufLim);
                continue;
            case 'z':  // subsecond part
            case 'g':
                buf = convertAndAddToBuff(msec, 3 /* subsecond precision */, buf, bufLim);
                continue;
            case 'F':  // AM/PM
                buf = addToBuff((tInfo->tm_hour >= 12) ? "PM" : "AM", buf, bufLim);
                continue;
            default:
              continue;
            }
        }
        if (buf == bufLim) break;
        *buf++ = *format;
    }
    return buf;
}
