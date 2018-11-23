//
//  base16.h
//  License++
//
//  Copyright © 2018-present Zuhd Web Services
//  https://zuhd.org
//
//  See https://github.com/zuhd-org/licensepp/blob/master/LICENSE 
//

#ifndef Base16_h
#define Base16_h

#include <string>

namespace licensepp {

///
/// \brief Base16 encoding wrappers
///
class Base16
{
public:
    static std::string decode(const std::string& encoded);
    static std::string encode(const std::string& raw);
};
}

#endif /* Base16_h */
