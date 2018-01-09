//
//  base64.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef Base64_h
#define Base64_h

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

#endif /* Base64_h */
