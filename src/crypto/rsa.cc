//
//  rsa.cc
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#include "src/external/Ripe.h"
#include "src/crypto/rsa.h"
#include "src/crypto/base64.h"

using namespace licensepp;

std::string RSA::encrypt(const std::string& data, const PublicKey& publicKey)
{
    return Ripe::encryptRSA(data, publicKey);
}

std::string RSA::decrypt(std::string& data, const PrivateKey& privateKey, const std::string& secret)
{
    return Ripe::decryptRSA(data, privateKey, true, false, secret);
}

std::string RSA::sign(const std::string& data, const PrivateKey& key, const std::string& secret)
{
    return Ripe::signRSA(data, key, secret);
}

bool RSA::verify(const std::string& data, const std::string& signHex, const PublicKey& key)
{
    return Ripe::verifyRSA(data, signHex, key);
}

bool RSA::verifyKeyPair(const PrivateKey& privateKey, const PublicKey& publicKey, const std::string& secret)
{
    try {
        std::string testData = "test_rsa";
        std::string encr = RSA::encrypt(testData, publicKey);
        std::string decr = Ripe::decryptRSA(encr, privateKey, false, false, secret);
        return testData == decr;
    } catch (const std::exception& e) {
        return false;
    }
}
