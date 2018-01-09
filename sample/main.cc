//
// License++
//
// Copyright © 2018-present Muflihun Labs
//
// https://muflihun.github.io/licensepp/
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
    
    LicenseManager licenseManager;
    if (!licenseFile.empty()) {
        std::ifstream stream(licenseFile);
        if (!stream.is_open()) {
            std::cerr << "Failed to open file " << licenseFile << std::endl;
        } else {
            
            std::string licenseKey = std::string((std::istreambuf_iterator<char>(stream)),
                                                 (std::istreambuf_iterator<char>()));
            stream.close();
            License license;
            license.load(licenseKey);
            if (!licenseManager.validate(&license, true, signature)) {
                std::cout << "License is not valid";
            } else {
                std::cout << "Licensed to " << license.licensee() << std::endl;
                std::cout << "Subscription is active until " << license.formattedExpiry() << std::endl << std::endl;
            }
        }
    } else {
        std::cout << "License file not provided" << std::endl;
        std::cout << "Usage: ./license-manager-sample --license <license-file> [--signature <signature>]" << std::endl;
    }
    return 0;
}
