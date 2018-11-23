//
//  issuing-authority.h
//  License++
//
//  Copyright © 2018-present Zuhd Web Services
//  https://zuhd.org
//
//  See https://github.com/zuhd-org/licensepp/blob/master/LICENSE 
//

#ifndef IssuingAuthority_h
#define IssuingAuthority_h

#include <string>
#include <license++/license.h>

namespace licensepp {

///
/// \brief License issuing authority
///
class IssuingAuthority
{
public:
    IssuingAuthority(const std::string& id, const std::string& name,
                     const std::string& keypair, unsigned int maxValidity,
                     bool active = true);

    IssuingAuthority(const IssuingAuthority&);
    IssuingAuthority& operator=(IssuingAuthority);

    inline std::string id() const
    {
        return m_id;
    }

    inline std::string name() const
    {
        return m_name;
    }

    inline bool active() const
    {
        return m_active;
    }

    inline unsigned int maxValidity() const
    {
        return m_maxValidity;
    }

    ///
    /// \brief Issue a new license
    /// \param licensee Name of license holder
    /// \param validityPeriod License validity in seconds. This must be <= issuing authority's max validity
    /// \param masterKey The decrypted master key
    /// \param secret Secret for issuing authority RSA keypair
    /// \param licenseeSignature Licensee signature to make license even more secure
    /// \return New license object
    /// \note Do not use this function directly. Use BaseLicenseManager::issue()
    ///
    License issue(const std::string& licensee,
                  unsigned int validityPeriod,
                  const std::string& masterKey,
                  const std::string& secret = "",
                  const std::string& licenseeSignature = "") const;

    ///
    /// \brief validate Validates license
    /// \param license License to validate
    /// \param masterKey The decrypted master key
    /// \param validateSignature Should signature be validated
    /// \param licenseeSignature If validateSignature what is the licensee signature
    /// \return True if license is valid and false if license is expired
    /// \note Do not use this function directly. Use BaseLicenseManager::validate()
    ///
    bool validate(const License* license,
                  const std::string& masterKey,
                  bool validateSignature,
                  const std::string& licenseeSignature = "") const;
private:
    std::string m_id;
    std::string m_name;
    std::string m_keypair;
    bool m_active;
    unsigned int m_maxValidity;
};
}

#endif /* IssuingAuthority_h */
