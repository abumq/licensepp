//
// License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//
// Usage: ./license-manager-sample --license <license-file> [--signature <signature>]

#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "license-manager.h"

int main(int argc, char* argv[])
{
    std::string licenseFile;
    std::string signature;
    for (int i = 0; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--license" && i < argc) {
            licenseFile = argv[++i];
        } else if (arg == "--signature" && i < argc) {
            signature = argv[++i];
        }
    }

    if (!licenseFile.empty()) {
        License license;
        try {
            if (license.loadFromFile(licenseFile)) {
                LicenseManager licenseManager;
                if (!licenseManager.validate(&license, true, signature)) {
                    std::cout << "License is not valid" << std::endl;
                } else {
                    std::cout << "Licensed to " << license.licensee() << std::endl;
                    std::cout << "Subscription is active until " << license.formattedExpiry() << std::endl << std::endl;
                }

            }
        } catch (LicenseException& e) {
            std::cerr << "Exception thrown " << e.what() << std::endl;
        }
    } else {
        std::cout << "License file not provided" << std::endl;
        std::cout << "Usage: ./license-manager-sample --license <license-file> [--signature <signature>]" << std::endl;
    }
    return 0;
}
