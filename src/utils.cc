//
//  utils.cc
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#include <ctime>
#include "src/utils.h"

using namespace licensepp;

unsigned long long Utils::nowUtc()
{
    std::time_t t = std::time(nullptr);
    std::tm* nowTm;
    nowTm = std::gmtime(&t);
    return nowTm != nullptr ? mktime(nowTm) : 0;
}
