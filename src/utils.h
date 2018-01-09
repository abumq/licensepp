//
//  utils.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef Utils_h
#define Utils_h

#include <chrono>

namespace license {

///
/// \brief Contains static utility functions used by Residue
///
class Utils
{
public:
    static inline unsigned long now()
    {
        return std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
    }

    static unsigned long long nowUtc();
};
}
#endif /* Utils_h */
