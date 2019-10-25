//
//  license-manager.h
//  License++ CLI
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef LicenseManagerKeyRegister_h
#define LicenseManagerKeyRegister_h

#include <string>
#include <vector>
#include <license++/issuing-authority.h>

class LicenseManagerKeyRegister
{
public:
    static const unsigned char LICENSE_MANAGER_SIGNATURE_KEY[];

    static const std::vector<licensepp::IssuingAuthority> LICENSE_ISSUING_AUTHORITIES;
};
#endif // LicenseManagerKeyRegister_h
