//
//  base16.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef Base16_h
#define Base16_h

#include <string>

namespace license {

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
