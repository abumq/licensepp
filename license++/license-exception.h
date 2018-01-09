//
//  license-exception.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef LicenseException_h
#define LicenseException_h

#include <exception>

namespace license {

///
/// \brief Exception thrown by all the residue helper and internal methods
///
class LicenseException : public std::runtime_error
{
public:
    ///
    /// \brief Main constructor
    ///
    LicenseException(const std::string& msg) : runtime_error(msg) {}

    virtual ~LicenseException() = default;
};

}
#endif /* LicenseException_h */
