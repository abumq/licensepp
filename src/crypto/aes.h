//
//  aes.h
//  License++
//
//  Copyright © 2018-present @abumq (Majid Q.)
//
//  See https://github.com/abumq/licensepp/blob/master/LICENSE
//

#ifndef LICENSEPP_AES_h
#define LICENSEPP_AES_h

#include <string>

namespace licensepp {

///
/// \brief AES crypto wrappers
///
class AES
{
public:
    ///
    /// \brief Decrypts AES-CBC
    /// \param raw Base64 encoded raw data (provided by ref)
    /// \param initVector Initialization vector (hex) for CBC. This is provided by reference in case it needs
    /// to be "normalized" i.e from AE2A => AE 2A for reability
    ///
    static std::string decrypt(std::string& raw, const std::string& hexKey, std::string& initVector);

    ///
    /// \brief This returns in following format:
    /// <pre>
    /// [iv]:[base64-encoded-cipher]
    /// </pre>
    ///
    static std::string encrypt(const std::string& raw, const std::string& hexKey, const std::string& initVector = "");

    ///
    /// \brief Generates a random key in bits.
    ///
    static std::string generateKey(unsigned int bits);
};
}

#endif /* LICENSEPP_AES_h */
