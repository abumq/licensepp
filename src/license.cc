//
//  license.cc
//  License++
//
//  Copyright © 2018-present Zuhd Web Services
//  https://zuhd.org
//
//  See https://github.com/zuhd-org/licensepp/blob/master/LICENSE 
//

#include <ctime>
#include <fstream>
#include <iterator>
#include <license++/license.h>
#include <license++/license-exception.h>
#include "src/crypto/base64.h"
#include "src/json-object.h"
#include "src/utils.h"

using namespace licensepp;

License::License() :
    m_issueDate(0),
    m_expiryDate(0)
{
}

License::License(const License& other):
    m_issueDate(other.m_issueDate),
    m_expiryDate(other.m_expiryDate),
    m_licensee(other.m_licensee),
    m_issuingAuthorityId(other.m_issuingAuthorityId),
    m_licenseeSignature(other.m_licenseeSignature)
{
}

License& License::operator=(License other)
{
    std::swap(m_issueDate, other.m_issueDate);
    std::swap(m_expiryDate, other.m_expiryDate);
    std::swap(m_licensee, other.m_licensee);
    std::swap(m_licenseeSignature, other.m_licenseeSignature);
    std::swap(m_issuingAuthorityId, other.m_issuingAuthorityId);
    return *this;
}

std::string License::formattedExpiry() const
{
    struct timeval tval;
    tval.tv_sec = static_cast<long>(m_expiryDate);
    return Utils::timevalToString(tval, "%d %b, %Y %H:%m UTC");
}

std::string License::toString()
{
    JsonObject::Json j;
    j["licensee"] = m_licensee;
    if (!m_licenseeSignature.empty()) {
        j["licensee_signature"] = m_licenseeSignature;
    }
    j["issue_date"] = m_issueDate;
    j["expiry_date"] = m_expiryDate;
    j["issuing_authority"] = m_issuingAuthorityId;
    j["authority_signature"] = m_authoritySignature;
    return Base64::encode(j.dump());
}

std::string License::raw() const
{
    JsonObject::Json j;
    j["licensee"] = m_licensee;
    if (!m_licenseeSignature.empty()) {
        j["licensee_signature"] = m_licenseeSignature;
    }
    j["issue_date"] = m_issueDate;
    j["expiry_date"] = m_expiryDate;
    j["issuing_authority"] = m_issuingAuthorityId;
    return j.dump();
}

bool License::load(const std::string& licenseBase64)
{
    try {
        const std::string jsonLicense = Base64::decode(licenseBase64);

        JsonObject::Json j = JsonObject::Json::parse(jsonLicense);
        setLicensee(j["licensee"].get<std::string>());
        setIssuingAuthorityId(j["issuing_authority"].get<std::string>());
        if (j.count("licensee_signature") > 0) {
            setLicenseeSignature(j["licensee_signature"].get<std::string>());
        }
        setIssueDate(j["issue_date"].get<unsigned long>());
        setExpiryDate(j["expiry_date"].get<unsigned long>());
        setAuthoritySignature(j["authority_signature"].get<std::string>());
        return true;
    } catch (const std::exception& e) {
        throw LicenseException("Failed to load the license: " + std::string(e.what()));
    }
    return false;
}

bool License::loadFromFile(const std::string& licenseFile)
{
    if (!licenseFile.empty()) {
        std::ifstream stream(licenseFile);
        if (!stream.is_open()) {
            std::cerr << "Failed to open file " << licenseFile << std::endl;
            return false;
        } else {
            std::string licenseContents = std::string((std::istreambuf_iterator<char>(stream)),
                                                      (std::istreambuf_iterator<char>()));
            stream.close();
            return load(licenseContents);
        }
    }
    return false;
}
