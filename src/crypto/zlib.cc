//
//  zlib.cc
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//
#include "src/external/Ripe.h"
#include "src/crypto/zlib.h"

using namespace license;

std::string ZLib::compress(const std::string& data)
{
    return Ripe::compressString(data);
}

std::string ZLib::decompress(const std::string& data)
{
    return Ripe::decompressString(data);
}

bool ZLib::compressFile(const std::string& gzoutFilename, const std::string& inputFile)
{
    return Ripe::compressFile(gzoutFilename, inputFile);
}
