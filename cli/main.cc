//
// License++
//
// Copyright © 2018-present Amrayn Web Services
// https://amrayn.com
//
// See https://github.com/amrayn/licensepp/blob/master/LICENSE
//

#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "licensing/license-manager.h"

void displayUsage() {
    std::cout << "USAGE: license-manager [--validate <file> --signature <signature>] [--issue --licensee <licensee> --signature <licensee_signature> --period <validation_period> --authority <issuing_authority> --passphrase <passphrase_for_issuing_authority> [--additional-payload <additional data>]]" << std::endl;
}

void displayVersion() {
    std::cout << "License Manager v1.0.0" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        displayVersion();
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        displayUsage();
        return 0;
    }

    std::string licenseFile;
    std::string signature;
    std::string licensee;
    std::string secret;
    std::string authority = "default";
    std::string additionalPayload;
    unsigned int period = 0U;
    bool doIssue = false;
    bool doValidate = false;

    for (int i = 0; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--validate" && i < argc) {
            licenseFile = argv[++i];
            doValidate = true;
        } else if (arg == "--signature" && i < argc) {
            signature = argv[++i];
        } else if (arg == "--issue" && i < argc) {
            doIssue = true;
        } else if (arg == "--licensee" && i < argc) {
            licensee = argv[++i];
        } else if (arg == "--period" && i < argc) {
            period = static_cast<unsigned int>(atoi(argv[++i]));
        } else if (arg == "--authority" && i < argc) {
            authority = argv[++i];
        } else if (arg == "--passphrase" && i < argc) {
            secret = argv[++i];
        } else if (arg == "--additional-payload" && i < argc) {
            additionalPayload = argv[++i];
        }
    }

    LicenseManager licenseManager;
    if (doValidate && !licenseFile.empty()) {
        License license;
        try {
            if (license.loadFromFile(licenseFile)) {
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
    } else if (doIssue) {
        const licensepp::IssuingAuthority* issuingAuthority = nullptr;
        for (const auto& a : LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES) {
            if (a.id() == authority) {
                issuingAuthority = &(a);
            }
        }
        if (issuingAuthority == nullptr) {
            std::cout << "Invalid issuing authority." << std::endl;
            return 1;
        }
        licensepp::License license = licenseManager.issue(licensee, period, issuingAuthority, secret, signature,additionalPayload);
        std::cout << license.toString() << std::endl;
        std::cout << "Licensed to " << license.licensee() << std::endl;
        std::cout << "Subscription is active until " << license.formattedExpiry() << std::endl << std::endl;
    } else {
        displayUsage();
    }
    return 0;
}
