//
//  base-license-manager.h
//  License++
//
//  Copyright © 2018-present @abumq (Majid Q.)
//
//  See https://github.com/abumq/licensepp/blob/master/LICENSE 
//

#ifndef BaseLicenseManager_h
#define BaseLicenseManager_h

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <license++/license.h>
#include <license++/license-exception.h>
#include <license++/issuing-authority.h>

namespace licensepp {

///
/// \brief Base license manager
///
/// This class is abstract license manager that needs to be inherited with proper
/// key register.
///
/// The key register must contains two static members.
///  1. static const unsigned char LICENSE_MANAGER_SIGNATURE_KEY[];
///  2. static const std::vector<licensepp::IssuingAuthority> LICENSE_ISSUING_AUTHORITIES;
///
/// Definitions must be provided as well.
///
/// A valid key register looks like as follows
/// <pre>
/// class LicenseKeysRegister
/// {
/// public:
///    static const unsigned char LICENSE_MANAGER_SIGNATURE_KEY[];
///
///    static const std::vector<IssuingAuthority> LICENSE_ISSUING_AUTHORITIES;
/// };
///
/// const unsigned char LicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY[] =
/// {
///    0x27, 0xD4, 0x91, 0x55, 0xE6, 0x6D, 0xC3, 0x11,
///    0x8D, 0xC0, 0x52, 0x0B, 0x2C, 0x9F, 0x84, 0xF3,
/// };
///
/// const std::vector<IssuingAuthority> LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES = {
///    IssuingAuthority("authority_id", "<authority name>", "<keypair>", 24U, true),
/// };
/// </pre>
///
/// And then your license manager for your software will look like this:
/// <pre>
/// class LicenseManager : public BaseLicenseManager<LicenseKeysRegister>
/// {
/// public:
///     LicenseManager()
///         : BaseLicenseManager()
///     {
///     }
/// };
/// </pre>
///
/// \see https://github.com/amrayn/licensepp/blob/master/sample/
///
template <class LicenseKeysRegister>
class BaseLicenseManager
{
public:
    BaseLicenseManager() = default;
    virtual ~BaseLicenseManager() = default;

    ///
    /// \brief Read and return issuing authority from license
    ///
    const IssuingAuthority* getIssuingAuthority(const License* license) const
    {
        if (license == nullptr) {
            return nullptr;
        }
        for (const auto& a : LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES) {
            if (a.id() == license->issuingAuthorityId()) {
                return &(a);
            }
        }
        return nullptr;
    }

    ///
    /// \brief Generates new license
    /// \param licensee Name of the licensee
    /// \param licenseeSignature Signature of the licensee
    /// \param validityPeriod Validity of license from time of creation.
    /// This is number of hours (for one year provide 8760, for one month [30 days] provide 720)
    /// \return New license object
    ///
    License issue(const std::string& licensee,
                  unsigned int validityPeriod,
                  const IssuingAuthority* issuingAuthority,
                  const std::string& issuingAuthoritySecret = "",
                  const std::string& licenseeSignature = "",
                  const std::string& additionalPayload = "") const
    {
        return issuingAuthority->issue(licensee, validityPeriod, keydec(),
                                       issuingAuthoritySecret, licenseeSignature,
                                       additionalPayload);
    }

    ///
    /// \brief Validates the license with current date
    /// \param Pointer to valid license object to change (for future use if needed)
    /// \param verifyLicenseeSignature Should we verify using or not after we know authority is correct
    /// \param licenseeSignature Plain signature from licensee
    /// \return True if license is still valid
    ///
    bool validate(const License* license,
                  bool verifyLicenseeSignature,
                  const std::string& licenseeSignature = "") const
    {
        const IssuingAuthority* issuingAuthority = getIssuingAuthority(license);
        if (issuingAuthority == nullptr) {
            throw LicenseException("Issuing authority [" +
                                   license->issuingAuthorityId() + "] not found");
        }
        if (!issuingAuthority->active()) {
            std::cerr << "WARN: Issuing authority "
                      << issuingAuthority->id()
                      << " cannot issue new licenses. Please update your license."
                      << std::endl;
        }
        return issuingAuthority->validate(license, keydec(), verifyLicenseeSignature, licenseeSignature);
    }
private:
    BaseLicenseManager(const BaseLicenseManager&) = delete;
    BaseLicenseManager& operator=(const BaseLicenseManager&) = delete;

    ///
    /// \brief Decode signature key
    ///
    inline std::string keydec() const
    {
        const std::string b16list = "0123456789ABCDEF";
        std::stringstream ss;
        for (auto i = 0; i < 16; ++i) {
            ss << b16list[LicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY[i] >> 4]
                    << b16list[LicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY[i] & 0xf];
        }
        return ss.str();
    }
};
}
#endif // BaseLicenseManager_h
