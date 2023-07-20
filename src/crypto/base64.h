//
//  base64.h
//  License++
//
//  Copyright © 2018-present @abumq (Majid Q.)
//
//  See https://github.com/abumq/licensepp/blob/master/LICENSE
//

#ifndef LICENSEPP_Base64_h
#define LICENSEPP_Base64_h

#include <string>

namespace licensepp {

///
/// \brief Base64 encoding wrappers
///
class Base64
{
public:
    static std::string decode(const std::string& encoded);
    static std::string encode(const std::string& raw);
};
}

#endif /* LICENSEPP_Base64_h */
