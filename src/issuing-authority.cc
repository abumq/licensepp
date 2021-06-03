//
//  issuing-authority.cc
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE
//

#include <cmath>
#include <iostream>
#include <license++/issuing-authority.h>
#include <license++/license.h>
#include <license++/license-exception.h>
#include "src/crypto/aes.h"
#include "src/crypto/base16.h"
#include "src/crypto/base64.h"
#include "src/crypto/rsa.h"
#include "src/utils.h"

using namespace licensepp;

IssuingAuthority::IssuingAuthority(const std::string& id,
                                   const std::string& name,
                                   const std::string& keyPair,
                                   unsigned int maxValidity,
                                   bool active) : IssuingAuthority::IssuingAuthority(id, name, "", "", maxValidity) {
    const auto separatorPos = keyPair.find(":");
    if (separatorPos == std::string::npos) {
        throw LicenseException("Issuing authority could not be loaded. Invalid keypair");
    }

    m_privateKey = Base64::decode(keyPair.substr(0, separatorPos));
    m_publicKey = Base64::decode(keyPair.substr(separatorPos + 1));
}

IssuingAuthority::IssuingAuthority(const std::string& id,
                                   const std::string& name,
                                   const std::string& privateKey,
                                   const std::string& publicKey,
                                   unsigned int maxValidity,
                                   bool active) :
    m_id(id),
    m_name(name),
    m_privateKey(Base64::decode(privateKey)),
    m_publicKey(Base64::decode(publicKey)),
    m_active(active),
    m_maxValidity(maxValidity)
{
    if (m_maxValidity < 24U) {
        std::cerr << "Could not activate issuing authority "
                  << id << ", it should be able to issue at least 24 hours license" << std::endl;
        m_active = false;
    }
}

IssuingAuthority::IssuingAuthority(const IssuingAuthority& other):
    m_id(other.m_id),
    m_name(other.m_name),
    m_privateKey(other.m_privateKey),
    m_publicKey(other.m_publicKey),
    m_active(other.m_active),
    m_maxValidity(other.m_maxValidity)
{
}

IssuingAuthority& IssuingAuthority::operator=(IssuingAuthority other)
{
    std::swap(m_id, other.m_id);
    std::swap(m_name, other.m_name);
    std::swap(m_privateKey, other.m_privateKey);
    std::swap(m_publicKey, other.m_publicKey);
    std::swap(m_active, other.m_active);
    std::swap(m_maxValidity, other.m_maxValidity);

    return *this;
}

License IssuingAuthority::issue(const std::string& licensee,
                                unsigned int validityPeriod,
                                const std::string& masterKey,
                                const std::string& secret,
                                const std::string& licenseeSignature,
                                const std::string& additionalPayload) const
{
    if (licensee.empty()) {
        throw LicenseException("Please provide valid licensee name and signature");
    }
    if (licensee.size() == 1 || licensee.size() > 255) {
        throw LicenseException("Licensee name too long. Please choose characters between 2-255");
    }
    if (validityPeriod < 24U) {
        throw LicenseException("License cannot be valid for less than 24 hours");
    }
    if (validityPeriod > maxValidity()) {
        throw LicenseException("License authority " + id() + " cannot issue license valid for more than " + std::to_string(maxValidity()) + " hours");
    }
    auto now = Utils::nowUtc();
    if (now == 0) {
        // This should never happen with gcc or clang compilers
        std::cerr << "WARN: Could not find UTC time, using local time" << std::endl;
        now = static_cast<unsigned long long>(Utils::now());
    }

    // -----------------------------------------------------------------------------------

    License license;
    license.setLicensee(licensee);
    license.setIssueDate(now);
    license.setExpiryDate(now + (validityPeriod * 3600));
    license.setIssuingAuthorityId(id());
    license.setAdditionalPayload(additionalPayload);
    if (!licenseeSignature.empty()) {
        try {
            license.setLicenseeSignature(Base16::encode(AES::encrypt(licenseeSignature, masterKey)));
        } catch (std::exception& e) {
            throw LicenseException("Failed to issue the license; " + std::string(e.what()));
        }
    }
    
    const RSA::PrivateKey key = RSA::loadPrivateKey(m_privateKey, secret);

    try {
        license.setAuthoritySignature(RSA::sign(license.raw(), key, secret));
    } catch (const std::exception& e) {
        std::cerr << "Failed to sign the license" + std::string(e.what()) << std::endl;
        throw LicenseException(e.what());
    }

    if (!validate(&license, masterKey, true, licenseeSignature)) {
        throw LicenseException("Failed to validate new license. Please report it @https://github.com/amrayn/licensepp");
    }
    return license;
}

bool IssuingAuthority::validate(const License* license,
                                const std::string& masterKey,
                                bool validateSignature,
                                const std::string& licenseeSignature) const
{
    bool result = false;
    try {
        RSA::PublicKey key = RSA::loadPublicKey(m_publicKey);

        result = RSA::verify(license->raw(), license->authoritySignature(), key);
        if (!result) {
            std::cerr << "Failed to verify the licensing authority" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to verify the licensing authority. " << e.what() << std::endl;
        return false;
    }
    auto now = Utils::nowUtc();
    if (now == 0) {
        // This should never happen with gcc or clang compilers
        std::cerr << "WARN: Could not find UTC time, using local time" << std::endl;
        now = static_cast<unsigned long long>(Utils::now());
    }

    auto diff = static_cast<int64_t>(license->expiryDate() - now);
    if (diff < 0) {
        int64_t hourDiff = ceil(llabs(diff) / 3600LL);
        std::cerr << "License was expired " << hourDiff << " hour"
                  << (hourDiff > 1 ? "s" : "") << " ago" << std::endl;
        return false;
    }

    if (!validateSignature && !license->licenseeSignature().empty()) {
        std::cerr << "Signature available on license, you should verify the signature" << std::endl;
        return false;
    }
    if (validateSignature && !license->licenseeSignature().empty()) {
        std::string decodedLicense = Base16::decode(license->licenseeSignature());
        std::string iv;
        auto ivPos = decodedLicense.find(":");
        if (ivPos != std::string::npos) {
            iv = decodedLicense.substr(0, ivPos);
        }
        result = result && AES::encrypt(licenseeSignature, masterKey, iv) == decodedLicense;
    }
    return result;
}
