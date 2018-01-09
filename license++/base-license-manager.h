//
//  base-license-manager.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef BaseLicenseManager_h
#define BaseLicenseManager_h

#include <iostream>
#include <string>
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
///    0x27, 0xD4, 0x91, 0x55, 0xE6, 0x6D, 0xC3, 0x11, 0x8D, 0xC0, 0x52, 0x0B, 0x2C, 0x9F, 0x84, 0xF3,
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
/// \see https://github.com/muflihun/licensepp/blob/master/sample/
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
        const IssuingAuthority* issuingAuthority = nullptr;
        for (const auto& a : LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES) {
            if (a.id() == license->issuingAuthorityId()) {
                issuingAuthority = &(a);
            }
        }
        return issuingAuthority;
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
                  const std::string& licenseeSignature = "") const
    {
        return issuingAuthority->issue(licensee, validityPeriod, keydec(), issuingAuthoritySecret, licenseeSignature);
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
        const IssuingAuthority* issuingAuthority = nullptr;
        // validate using license's authortiy
        for (const IssuingAuthority& currAuthority : LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES) {
            if (currAuthority.id() == license->issuingAuthorityId()) {
                if (!currAuthority.active()) {
                    std::cerr << "Issuing authority " << license->issuingAuthorityId()
                                  << " cannot issue new licenses. Please update your license." << std::endl;
                }
                issuingAuthority = &currAuthority;
                break;
            }
        }
        if (issuingAuthority == nullptr) {
            throw LicenseException("Issuing authority [" + license->issuingAuthorityId() + "] not found");
        }
        return issuingAuthority->validate(license, keydec(), verifyLicenseeSignature, licenseeSignature);
    }
private:
    ///
    /// \brief Decode signature key
    ///
    inline std::string keydec() const
    {
        const std::string b16list = "0123456789ABCDEF";
        const unsigned char* key = LicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY;
        std::stringstream ss;
        for (auto i = 0; i < 16; ++i) {
            ss << b16list[key[i] >> 4] << b16list[key[i] & 0xf];
        }
        return ss.str();
    }
};
}
#endif // BaseLicenseManager_h
