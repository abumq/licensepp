//
//  license-manager.h
//  License++ CLI
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef LicenseManager_h
#define LicenseManager_h

#include <license++/base-license-manager.h>
#include <license++/license.h>
#include <license++/issuing-authority.h>
#include "license-manager-key-register.h"

using namespace licensepp;

class LicenseManager : public BaseLicenseManager<LicenseManagerKeyRegister>
{
public:
    LicenseManager()
        : BaseLicenseManager()
    {
    }
};
#endif /* LicenseManager_h */
