//
//  Ripe
//
//  Copyright 2017-present @abumq (Majid Q.)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <cerrno>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>

#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/pem.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>

#include <zlib.h>

#include "Ripe.h"

#define RIPE_UNUSED(x) (void)x

using namespace CryptoPP;

const std::string Ripe::PACKET_DELIMITER      = "\r\n\r\n";
const std::size_t Ripe::PACKET_DELIMITER_SIZE = Ripe::PACKET_DELIMITER.size();
const char        Ripe::DATA_DELIMITER        = ':';
const int         Ripe::DEFAULT_RSA_LENGTH    = 2048;
const int         Ripe::ZLIB_BUFFER_SIZE      = 32768;
const int         Ripe::AES_BLOCK_SIZE             = AES::BLOCKSIZE;
const std::string Ripe::BASE64_CHARS          = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const std::string Ripe::PRIVATE_RSA_ALGORITHM = "AES-256-CBC";

bool loadPrivateKey(const std::string& key, RSA::PrivateKey& keyOut, const std::string& secret)
{
    StringSource source(key, true);
    if (secret.empty()) {
        PEM_Load(source, keyOut);
    } else {
        PEM_Load(source, keyOut, secret.data(), secret.size());
    }
    AutoSeededRandomPool prng;
    return keyOut.Validate(prng, 3);
}

bool loadPublicKey(const std::string& key, RSA::PublicKey& keyOut)
{
    StringSource source(key, true);
    PEM_Load(source, keyOut);
    AutoSeededRandomPool prng;
    return keyOut.Validate(prng, 3);
}

std::string Ripe::encryptRSA(const std::string& data, const std::string& publicKeyPEM)
{
    RSA::PublicKey publicKey;
    bool rsaKeyValid = loadPublicKey(publicKeyPEM, publicKey);
    if (!rsaKeyValid) {
        throw std::invalid_argument("Could not load public key");
    }

    RSAES<PKCS1v15>::Encryptor e(publicKey);

    std::string result;
    AutoSeededRandomPool rng;
    StringSource ss(data, true,
        new PK_EncryptorFilter(rng, e,
            new StringSink(result)
       )
    );
    RIPE_UNUSED(ss);
    return result;
}

std::string Ripe::encryptRSA(std::string& data, const std::string& key, const std::string& outputFile, bool isRaw)
{
    try {
        std::string encryptedData = Ripe::encryptRSA(data, key);
        if (!isRaw) {
            encryptedData = Ripe::base64Encode(encryptedData);
        }
        if (!outputFile.empty()) {
            std::ofstream out(outputFile.c_str());
            out.write(encryptedData.c_str(), encryptedData.size());
            out.flush();
            out.close();
            return "";
        }
        return encryptedData;
    } catch (const std::exception& e) {
        throw e;
    }
}

std::string Ripe::decryptRSA(const std::string& data, const std::string& privateKeyPEM, const std::string& secret)
{
    RSA::PrivateKey privateKey;
    bool rsaKeyValid = loadPrivateKey(privateKeyPEM, privateKey, secret);
    if (!rsaKeyValid) {
        throw std::invalid_argument("Could not load private key");
    }

    std::string result;
    AutoSeededRandomPool rng;
    RSAES<PKCS1v15>::Decryptor d(privateKey);

    StringSource ss(data, true,
        new PK_DecryptorFilter(rng, d,
            new StringSink(result)
       )
    );
    RIPE_UNUSED(ss);
    return result;
}

std::string Ripe::decryptRSA(std::string& data, const std::string& key, bool isBase64, bool isHex, const std::string& secret)
{
    if (isBase64) {
        data = Ripe::base64Decode(data);
    }

    if (isHex) {
        data = Ripe::hexToString(data);
    }
    return Ripe::decryptRSA(data, key, secret);
}

bool Ripe::verifyRSA(const std::string& data, const std::string& signatureHex, const std::string& publicKeyPEM)
{
    RSA::PublicKey publicKey;
    bool rsaKeyValid = loadPublicKey(publicKeyPEM, publicKey);
    if (!rsaKeyValid) {
        throw std::invalid_argument("Could not load public key");
    }
    std::string decodedSignature = Ripe::hexToString(signatureHex);
    bool result = false;
    RSASS<PKCS1v15,SHA1>::Verifier verifier(publicKey);
    StringSource ss2(decodedSignature + data, true,
                     new SignatureVerificationFilter(verifier,
                                                     new ArraySink((RipeByte*)&result, sizeof(result))));
    return result;
}

std::string Ripe::signRSA(const std::string& data, const std::string& privateKeyPEM, const std::string& privateKeySecret)
{
    RSA::PrivateKey privateKey;
    bool rsaKeyValid = loadPrivateKey(privateKeyPEM, privateKey, privateKeySecret);
    if (!rsaKeyValid) {
        throw std::invalid_argument("Could not load private key");
    }

    // sign message
    std::string signature;
    RSASS<PKCS1v15,SHA1>::Signer signer(privateKey);
    AutoSeededRandomPool rng;

    StringSource ss(data, true,
                    new SignerFilter(rng, signer,
                                     new HexEncoder(
                                         new StringSink(signature))));
    return signature;
}

bool Ripe::writeRSAKeyPair(const std::string& publicFile, const std::string& privateFile, int length, const std::string& secret)
{
    bool result = true;
    KeyPair keypair = Ripe::generateRSAKeyPair(length, secret);
    if (keypair.privateKey.size() > 0 && keypair.publicKey.size() > 0) {
        std::ofstream fs(privateFile.c_str(), std::ios::out);
        if (fs.is_open()) {
            fs.write(keypair.privateKey.c_str(), keypair.privateKey.size());
            fs.close();
        } else {
            throw std::runtime_error(
                        std::string("Unable to open private file for writing [" + privateFile + "] " + std::strerror(errno)).data()
                        );
            result = false;
        }
        fs.open(publicFile.c_str(), std::ios::out);
        if (fs.is_open()) {
            fs.write(keypair.publicKey.c_str(), keypair.publicKey.size());
            fs.close();
            result = result && true;
        } else {
            throw std::runtime_error(
                        std::string("Unable to open public file for writing [" + publicFile + "] " + std::strerror(errno)).data()
                        );
            result = false;
        }
    }
    if (!result) {
        throw std::runtime_error("Failed to generate key pair.");
    }
    return result;
}

std::string Ripe::generateRSAKeyPairBase64(int length, const std::string& secret)
{
    Ripe::KeyPair pair = Ripe::generateRSAKeyPair(length, secret);
    if (pair.privateKey.empty() || pair.publicKey.empty()) {
        throw std::runtime_error("Failed to generate key pair.");
    }
    return Ripe::base64Encode(pair.privateKey) + ":" + Ripe::base64Encode(pair.publicKey);
}

Ripe::KeyPair Ripe::generateRSAKeyPair(unsigned int length, const std::string& secret)
{
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, length);
    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    Ripe::KeyPair pair;
    {
        StringSink snk(pair.privateKey);
        if (secret.empty()) {
            PEM_Save(snk, privateKey);
        } else {
            PEM_Save(snk, privateKey, rng, PRIVATE_RSA_ALGORITHM, secret.data(), secret.size());
        }
        snk.MessageEnd();
    }
    {
        StringSink snk(pair.publicKey);
        PEM_Save(snk, publicKey);
        snk.MessageEnd();
    }
    return pair;
}

std::string Ripe::base64Encode(const std::string& input)
{
    std::string encoded;
    StringSource ss(input, true, new Base64Encoder(
                        new StringSink(encoded), false /* insert line breaks */)
                    );
    RIPE_UNUSED(ss);
    return encoded;
}

std::string Ripe::base64Decode(const std::string& base64Encoded)
{
    std::string decoded;
    StringSource ss(base64Encoded, true, new Base64Decoder(
                        new StringSink(decoded))
                    );
    RIPE_UNUSED(ss);
    return decoded;
}

std::string Ripe::generateNewKey(int length)
{
    if (!(length == 16 || length == 24 || length == 32)) {
        throw std::invalid_argument( "Invalid key length. Acceptable lengths are 16, 24 or 32" );
    }
    AutoSeededRandomPool rnd;
    SecByteBlock key(length);
    rnd.GenerateBlock(key, key.size());
    std::string s;
    HexEncoder hex(new StringSink(s));
    hex.Put(key.data(), key.size());
    hex.MessageEnd();
    return s;
}

std::string Ripe::encryptAES(const std::string& buffer, const RipeByte* key, std::size_t keySize, std::vector<RipeByte>& iv)
{
    SecByteBlock keyBlock(key, keySize);

    RipeByte ivArr[Ripe::AES_BLOCK_SIZE] = {0};

    if (iv.empty()) {
        AutoSeededRandomPool rnd;
        rnd.GenerateBlock(ivArr, sizeof ivArr);
    } else {
        for (std::size_t i = 0; i < iv.size(); ++i) {
            ivArr[i] = iv.at(i);
        }
    }


    std::string cipher;

    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(keyBlock, keyBlock.size(), ivArr);

    if (iv.empty()) {
        // store for user
        iv.resize(sizeof ivArr);
        std::copy(ivArr, ivArr + Ripe::AES_BLOCK_SIZE, iv.begin());
    }

    // The StreamTransformationFilter adds padding as required.
    StringSource ss(buffer, true,
                    new StreamTransformationFilter(e, new StringSink(cipher))
                    );
    RIPE_UNUSED(ss);
    return cipher;
}

std::string Ripe::encryptAES(std::string& data, const std::string& hexKey, const std::string& clientId, const std::string& outputFile, const std::string& ivec)
{
    std::stringstream ss;
    if (!outputFile.empty()) {
        std::vector<RipeByte> iv;
        if (!ivec.empty()) {
            RipeByte* ivBytes = reinterpret_cast<RipeByte*>(const_cast<char*>(ivec.data()));
            iv = Ripe::RipeByteToVec(ivBytes);
        }
        std::string encrypted = Ripe::encryptAES(data, hexKey, iv);

        std::ofstream out(outputFile.c_str());
        out << encrypted.data();
        out.close();
        ss << "IV: " << std::hex << std::setfill('0');
        for (std::vector<RipeByte>::const_iterator it = iv.begin(); it < iv.end(); ++it) {
            ss << std::setw(2) << static_cast<unsigned int>(*it);
        }
        ss << std::endl;
    } else {
        ss << Ripe::prepareData(data, hexKey, clientId.c_str(), ivec);
    }
    return ss.str();
}

std::string Ripe::decryptAES(const std::string& data, const RipeByte* key, std::size_t keySize, std::vector<RipeByte>& iv)
{
    std::string result;
    SecByteBlock keyBlock(key, keySize);

    RipeByte ivArr[Ripe::AES_BLOCK_SIZE] = {0};
    std::copy(iv.begin(), iv.end(), ivArr);

    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(keyBlock, keyBlock.size(), ivArr);

    StringSource ss(data, true,
                new StreamTransformationFilter( d, new StringSink(result))
                );
    RIPE_UNUSED(ss);
    return result;
}

std::string Ripe::decryptAES(std::string& data, const std::string& hexKey, std::string& ivec, bool isBase64, bool isHex)
{
    if (ivec.empty() && isBase64) {
        // Extract IV from data
        std::size_t pos = data.find_first_of(':');
        if (pos == 32) {
            ivec = data.substr(0, pos);
            Ripe::normalizeHex(ivec);
            data = data.substr(pos + 1);
            pos = data.find_first_of(':');
            if (pos != std::string::npos) {
                // We ignore clientId which is = data.substr(0, pos);
                data = data.substr(pos + 1);
            }
        }
    }
    if (ivec.size() == 32) {
        // Condensed form needs to be normalized
        Ripe::normalizeHex(ivec);
    }

    RipeByte* iv = reinterpret_cast<RipeByte*>(const_cast<char*>(ivec.data()));
    std::vector<RipeByte> ivHex = Ripe::RipeByteToVec(iv);

    if (isBase64) {
        data = Ripe::base64Decode(data);
    }
    if (isHex) {
        data = Ripe::hexToString(data);
    }
    return Ripe::decryptAES(data, reinterpret_cast<const RipeByte*>(Ripe::hexToString(hexKey).c_str()), hexKey.size() / 2, ivHex);
}

bool Ripe::compressFile(const std::string& gzFilename, const std::string& inputFile)
{
    gzFile out = gzopen(gzFilename.c_str(), "wb");
    if (!out) {
        throw std::runtime_error(
                    std::string("Unable to open file for writing [" + gzFilename + "] " + std::strerror(errno)).data()
                    );
        return false;
     }
    char buff[BUFSIZ];
    std::FILE* in = std::fopen(inputFile.c_str(), "rb");
    std::size_t nRead = 0;
    while((nRead = std::fread(buff, sizeof(char), BUFSIZ, in)) > 0) {
        int RipeBytes_written = gzwrite(out, buff, nRead);
        if (RipeBytes_written == 0) {
           int err_no = 0;
           throw std::runtime_error(
                       std::string("Error during compression " + std::string(gzerror(out, &err_no))).data()
                       );
           gzclose(out);
           return false;
        }
    }
    gzclose(out);
    std::fclose(in);
    return true;
}

std::string Ripe::compressString(const std::string& str)
{
    int compressionlevel = Z_BEST_COMPRESSION;
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK) {
        throw std::runtime_error("Unable to initialize zlib deflate");
    }

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
    zs.avail_in = str.size();

    int ret;
    char outbuffer[ZLIB_BUFFER_SIZE];
    std::string outstring;

    // retrieve the compressed RipeBytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << (zs.msg != NULL ? zs.msg : "no msg");
        throw std::runtime_error(oss.str());
    }

    return outstring;
}

std::string Ripe::decompressString(const std::string& str)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK) {
        throw std::runtime_error("Unable to initialize zlib inflate");
    }

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
    zs.avail_in = str.size();

    int ret;
    char outbuffer[ZLIB_BUFFER_SIZE];
    std::string outstring;

    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << (zs.msg != NULL ? zs.msg : "no msg");
        throw std::runtime_error(oss.str());
    }

    return outstring;
}

std::string Ripe::sha256Hash(const std::string& data)
{
    std::string digest;
    SHA256 hasher;
    StringSource ss(data, true, new HashFilter(hasher, new HexEncoder(new StringSink(digest))));
    return digest;
}

std::string Ripe::sha512Hash(const std::string& data)
{
    std::string digest;
    CryptoPP::SHA512 hasher;
    StringSource ss(data, true, new HashFilter(hasher, new HexEncoder(new StringSink(digest))));
    return digest;
}

std::string Ripe::prepareData(const std::string& data, const std::string& hexKey, const char* clientId, const std::string& ivec)
{
    std::vector<RipeByte> iv;
    if (!ivec.empty()) {
        std::string ivector(ivec);
        if (ivector.size() == 32) {
            Ripe::normalizeHex(ivector);
        }
        RipeByte* ivBytes = reinterpret_cast<RipeByte*>(const_cast<char*>(ivector.data()));
        iv = Ripe::RipeByteToVec(ivBytes);
    }
    std::string encrypted = Ripe::encryptAES(data, hexKey, iv);
    // Encryption Base64 encoding
    std::string base64Encoded = Ripe::base64Encode(encrypted);

    // IV Hex
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (std::vector<RipeByte>::const_iterator it = iv.begin(); it < iv.end(); ++it) {
        ss << std::setw(2) << static_cast<unsigned int>(*it);
    }
    ss << Ripe::DATA_DELIMITER;
    if (strlen(clientId) > 0) {
        ss << clientId << Ripe::DATA_DELIMITER;
    }
    ss << base64Encoded;
    std::stringstream fss;
    fss << ss.str() << PACKET_DELIMITER;
    return fss.str();
}

bool Ripe::normalizeHex(std::string& iv)
{
    if (iv.size() == 32) {
        for (int j = 2; j < 32 + 15; j += 2) {
            iv.insert(j, " ");
            j++;
        }
        return true;
    }
    return false;
}

std::string Ripe::vecToString(const std::vector<RipeByte>& iv)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (std::vector<RipeByte>::const_iterator it = iv.begin(); it < iv.end(); ++it) {
        ss << std::setw(2) << static_cast<unsigned int>(*it);
    }
    return ss.str();
}

std::vector<RipeByte> Ripe::RipeByteToVec(const RipeByte* b)
{
    std::vector<RipeByte> hexData;

    std::istringstream ss(reinterpret_cast<const char*>(b));

    unsigned int c;
    while (ss >> std::hex >> c) {
        hexData.push_back(c);
    }
    return hexData;
}

std::string Ripe::hexToString(const std::string& hex)
{
    std::string result;
    StringSource ss(hex, true,
        new HexDecoder(
            new StringSink(result)
        )
    );
    RIPE_UNUSED(ss);
    return result;
}

std::string Ripe::stringToHex(const std::string& raw)
{
    std::string result;
    StringSource ss(raw, true,
        new HexEncoder(
            new StringSink(result)
        )
    );
    RIPE_UNUSED(ss);
    return result;
}

std::size_t Ripe::expectedDataSize(std::size_t plainDataSize, std::size_t clientIdSize)
{
    std::size_t dataSize = 32 /* IV */
            + sizeof(DATA_DELIMITER) /* : */
            + (clientIdSize > 0 ? clientIdSize + sizeof(DATA_DELIMITER) : 0)
            + expectedBase64Length(expectedAESCipherLength(plainDataSize));
    return dataSize + PACKET_DELIMITER_SIZE;
}

std::string Ripe::version()
{
    return RIPE_VERSION;
}
