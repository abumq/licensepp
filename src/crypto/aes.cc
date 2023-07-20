//
//  aes.cc
//  License++
//
//  Copyright © 2018-present @abumq (Majid Q.)
//
//  See https://github.com/abumq/licensepp/blob/master/LICENSE
//

#include <Ripe.h>

#include "src/crypto/aes.h"

using namespace licensepp;

std::string AES::decrypt(std::string& raw, const std::string& key, std::string& iv)
{
    return Ripe::decryptAES(raw, key, iv, true);
}

std::string AES::encrypt(const std::string& plain, const std::string& key, const std::string& iv)
{
    std::string plainCopy(plain);
    return Ripe::encryptAES(plainCopy, key, "", "", iv);
}

std::string AES::generateKey(unsigned int bits)
{
    return Ripe::generateNewKey(bits / 8);
}
