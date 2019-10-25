//
//  base64.cc
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#include "src/external/Ripe.h"
#include "src/crypto/base64.h"

using namespace licensepp;

std::string Base64::decode(const std::string& encoded)
{
    return Ripe::base64Decode(encoded);
}

std::string Base64::encode(const std::string& raw)
{
    return Ripe::base64Encode(raw);
}
