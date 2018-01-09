//
//  license.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef License_h
#define License_h

#include <string>

namespace licensepp {

///
/// \brief License model
///
class License
{
public:
    License();
    License(const License&);
    License& operator=(License);

    inline void setLicensee(const std::string& licensee)
    {
        m_licensee = licensee;
    }

    inline void setIssuingAuthorityId(const std::string& issuingAuthorityId)
    {
        m_issuingAuthorityId = issuingAuthorityId;
    }

    inline void setLicenseeSignature(const std::string& licenseeSignature)
    {
        m_licenseeSignature = licenseeSignature;
    }

    inline void setAuthoritySignature(const std::string& authoritySignature)
    {
        m_authoritySignature = authoritySignature;
    }

    inline void setExpiryDate(unsigned long expiryDate)
    {
        m_expiryDate = expiryDate;
    }

    inline void setIssueDate(unsigned long issueDate)
    {
        m_issueDate = issueDate;
    }

    inline std::string licensee() const
    {
        return m_licensee;
    }

    inline std::string issuingAuthorityId() const
    {
        return m_issuingAuthorityId;
    }

    inline std::string licenseeSignature() const
    {
        return m_licenseeSignature;
    }

    inline std::string authoritySignature() const
    {
        return m_authoritySignature;
    }

    inline unsigned long expiryDate() const
    {
        return m_expiryDate;
    }

    inline unsigned long issueDate() const
    {
        return m_issueDate;
    }

    std::string toString();

    ///
    /// \brief Returns raw format of license
    ///
    std::string raw() const;

    ///
    /// \brief Returns expiry date in <pre>%d %b, %Y %H:%m UTC</pre> format
    ///
    std::string formattedExpiry() const;

    ///
    /// \brief Loads itself from base64 input
    /// \throws LicenseException if license is invalid
    ///
    bool load(const std::string& licenseBase64);

    ///
    /// \brief Loads itself from license file containing base64 license
    /// \throws LicenseException if license is invalid
    ///
    bool loadFromFile(const std::string& licensefile);

protected:

    unsigned long m_issueDate;
    unsigned long m_expiryDate;

    std::string m_licensee;
    std::string m_issuingAuthorityId;
    std::string m_licenseeSignature;
    std::string m_authoritySignature;
};
}

#endif /* License_h */
