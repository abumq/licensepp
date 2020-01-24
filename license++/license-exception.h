//
//  license-exception.h
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef LicenseException_h
#define LicenseException_h

#include <stdexcept>

namespace licensepp {

class LicenseExceptionBase {};
///
/// \brief Exception thrown by all the license++ functions
///
class LicenseException : public LicenseExceptionBase, public std::runtime_error
{
public:
    ///
    /// \brief Main constructor
    ///
    LicenseException(const std::string& msg) : std::runtime_error(msg) {}

    virtual ~LicenseException() = default;
};

}
#endif /* LicenseException_h */
