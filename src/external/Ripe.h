//
//  Ripe.cc
////
//  Copyright © 2017-present Amrayn Web Services
//  https://amrayn.com
//  https://muflihun.com
//  https://github.com/amrayn
//
//  [This is custom version of Ripe for License++]
//

#ifndef Ripe_h
#define Ripe_h

#include <string>
#include <vector>
#include <algorithm>

typedef unsigned char RipeByte;

#ifdef RIPE_DLL
#ifdef RIPE_EXPORTS
#define RIPE_API __declspec(dllexport)
#else
#define RIPE_API __declspec(dllimport)
#endif
#else
#define RIPE_API
#endif

///
/// \brief The Ripe class is core of Ripe library and contains all
/// the required cryptography API supported by Ripe
///
RIPE_API class Ripe {
public:

    ///
    /// \brief Constant for end of data delimiter
    ///
    static const std::string PACKET_DELIMITER;

    ///
    /// \brief Size of PACKET_DELIMITER
    /// \see PACKET_DELIMITER
    ///
    static const std::size_t PACKET_DELIMITER_SIZE;

    ///
    /// \brief Data delimiter for prepared data
    /// \see prepareData(const char*, const std::string&, const char*)
    ///
    static const char DATA_DELIMITER;

    ///
    /// \brief Constant value default rsa length
    ///
    static const int DEFAULT_RSA_LENGTH;

    ///
    /// \brief Constant value for AES block size
    ///
    static const int AES_BLOCK_SIZE;

    ///
    /// \brief Possible base64 characters
    ///
    static const std::string BASE64_CHARS;

    ///
    /// \brief Algorithm for private rsa key
    ///
    static const std::string PRIVATE_RSA_ALGORITHM;

    ///
    /// \brief Buffer size for zlib
    ///
    static const int ZLIB_BUFFER_SIZE;

    ///
    /// \brief RSA Key pair
    ///
    struct KeyPair {
        ///
        /// \brief Private key of this pair
        ///
        std::string privateKey;

        ///
        /// \brief Public key of this pair
        ///
        std::string publicKey;
    };

    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             AES                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                             CRYPTO                              *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief Encrypts data of length with symmetric key of size = keySize with specified initialization vector
    ///
    static std::string encryptAES(const std::string& data, const RipeByte* key, std::size_t keySize, std::vector<RipeByte>& iv);

    ///
    /// \brief Decrypts data of specified length with specified key and initialization vector
    ///
    static std::string decryptAES(const std::string& data, const RipeByte* key, std::size_t keySize, std::vector<RipeByte>& iv);

    ///
    /// \brief Generate random AES key
    /// \param length Length of key, must be 16, 24 or 32
    /// \return Hexadecimal value of key
    ///
    static std::string generateNewKey(int length);




    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             AES                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                            HELPERS                              *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief encryptAES Encrypts data with provided symmetric key
    /// \param outputFile Optional, if provided instead of printing it to console data is saved to file and IV is printed on console
    ///
    static std::string encryptAES(std::string& data, const std::string& hexKey, const std::string& clientId, const std::string& outputFile, const std::string& iv = "");

    ///
    /// \brief Helper function that takes hex key
    /// \see encryptAES(std::string& data, const std::string& hexKey, const std::string& clientId, const std::string& outputFile)
    ///
    inline static std::string encryptAES(const std::string& buffer, const std::string& hexKey, std::vector<RipeByte>& iv)
    {
        return encryptAES(buffer, reinterpret_cast<const RipeByte*>(hexToString(hexKey).c_str()), hexKey.size() / 2, iv);
    }

    ///
    /// \brief decryptAES Decrypts data using specified symmetric key.
    /// \param isBase64 If true, first base64 decoding is done on data and then decryption is processed
    /// \param isHex If true, hex string is decoded. If base64 is true, base64 decoding is done before hex decoding
    ///
    static std::string decryptAES(std::string& data, const std::string& hexKey, std::string& iv, bool isBase64 = false, bool isHex = false);


    ///
    /// \brief Exceptect size of AES cipher when plainDataSize size data is encrypted
    ///
    inline static std::size_t expectedAESCipherLength(std::size_t plainDataSize)
    {
        return (plainDataSize / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }

    ///
    /// \brief normalizeIV If IV with no space is provided e.g, <pre>67e56fee50e22a8c2ba05c0fb2932bfa:</pre> normalized IV
    /// is <pre>67 e5 6f ee 50 e2 2a 8c 2b a0 5c 0f b2 93 2b fa:</pre>
    ///
    static bool normalizeHex(std::string& iv);



    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             RSA                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                            CRYPTO                               *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/


    ///
    /// \brief Encrypts data of length = dataLength using RSA key and puts it in destination
    ///
    /// Ripe uses PKCS #1 v1.5 padding scheme
    ///
    /// \return The size of the encrypted data. On error -1 is returned. use printLastError(const char*) to see the error details
    ///
    static std::string encryptRSA(const std::string& data, const std::string& publicKeyPEM);

    ///
    /// \brief Decrypts encryptedData of length dataLength with RSA key and puts result in destination
    ///
    /// Ripe uses PKCS #1 v1.5 padding scheme
    ///
    /// \return The size of the recovered plaintext. On error -1 is returned. use printLastError(const char* name) to see the error details
    ///
    static std::string decryptRSA(const std::string& data, const std::string& privateKeyPEM, const std::string& secret = "");

    ///
    /// \brief Verifies the data is signed by associated private key
    /// \param data The data to verify
    /// \param signatureHex Signature in hex format
    /// \param publicKeyPEM Public key to verify the data from
    ///
    static bool verifyRSA(const std::string& data, const std::string& signatureHex, const std::string& publicKeyPEM);

    ///
    /// \brief Signs the data with private key
    /// \param data The data to sign
    /// \param privateKeyPEM private key to sign the data with
    /// \param secret Private key secret
    /// \return Hex format signature
    ///
    static std::string signRSA(const std::string& data, const std::string& privateKeyPEM, const std::string& secret = "");

    ///
    /// \brief Generate key pair and returns KeyPair
    /// \param length Length of the key (2048 for 256-bit key, ...)
    /// \param secret Password for private RSA key (if any)
    /// \see KeyPair
    ///
    static KeyPair generateRSAKeyPair(unsigned int length = DEFAULT_RSA_LENGTH, const std::string& secret = "");




    /*****************************************************************************************************/

                /*******************************************************************\
                 *                             RSA                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           HELPERS                               *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief Maximum size of RSA block with specified key size
    /// \param keySize 2048 for 256-bit key, ...
    ///
    inline static unsigned int maxRSABlockSize(std::size_t keySize)
    {
        return (keySize / 8) - 11;
    }

    ///
    /// \brief Minimum size of RSA key to encrypt data of dataSize size
    ///
    inline static unsigned int minRSAKeySize(std::size_t dataSize)
    {
        return (dataSize + 11) * 8;
    }

    ///
    /// \brief encryptRSA Encrypts using RSA key
    /// \param outputFile Optional, if provided instead of printing it to console data is saved to file
    /// \param isRaw Outputs raw data
    ///
    static std::string encryptRSA(std::string& data, const std::string& key, const std::string& outputFile, bool isRaw = false);

    ///
    /// \brief decryptRSA Decrypts using RSA key
    /// \param isHex If true, hex string is decoded. If base64 is true, base64 decoding is done before hex decoding
    /// \param isBase64 If true, first base64 decoding is done on data and then decryption is processed
    ///
    static std::string decryptRSA(std::string& data, const std::string& key, bool isBase64, bool isHex = false, const std::string& secret = "");

    ///
    /// \brief writeRSAKeyPair Writes RSA key pair to public and private file paths.
    /// \param publicFile Output path for public file. It must be wriable.
    /// \param privateFile Output path for private file. It must be writable.
    /// \param length Length of the key (2048 for 256-bit key, ...)
    /// \param secret Password for private RSA key (if any)
    ///
    static bool writeRSAKeyPair(const std::string& publicFile, const std::string& privateFile, int length = DEFAULT_RSA_LENGTH, const std::string& secret = "");

    ///
    /// \brief generateRSAKeyPair Generates RSA key pair and returns colon seperated base64 where first part is private key and second part is public key.
    ///
    static std::string generateRSAKeyPairBase64(int length = DEFAULT_RSA_LENGTH, const std::string& secret = "");



    /*****************************************************************************************************/

                /*******************************************************************\
                 *                            MISC                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           Base64                                *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief Decodes encoded base64
    ///
    static std::string base64Decode(const std::string& base64Encoded);

    ///
    /// \brief Encodes input of length to base64 encoding
    ///
    static std::string base64Encode(const std::string& binaryData);

    ///
    /// \brief expectedBase64Length Returns expected base64 length
    /// \param n Length of input (plain data)
    ///
    inline static std::size_t expectedBase64Length(std::size_t n)
    {
        return ((4 * n / 3) + 3) & ~0x03;
    }

    ///
    /// \brief Finds whether data is base64 encoded. This is done
    /// by finding non-base64 character. So it is not necessary
    /// a valid base64 encoding.
    ///
    inline static bool isBase64(const std::string& data)
    {
        return data.find_first_not_of(BASE64_CHARS) == std::string::npos;
    }


    /*****************************************************************************************************/

                /*******************************************************************\
                 *                            MISC                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                            ZLib                                 *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    /**
     * @brief Compress input file (path) and create new file
     * @param gzFilename Output file path
     * @param inputFile Input file path
     * @return True if successful, otherwise false
     */
    static bool compressFile(const std::string& gzFilename, const std::string& inputFile);

    /**
     * @brief Compresses string using zlib (inflate)
     * @param str Input plain text
     * @return Raw output (binary)
     */
    static std::string compressString(const std::string& str);

    /**
     * @brief Decompresses string using zlib (deflate)
     * @param str Raw input
     * @return Plain output
     */
    static std::string decompressString(const std::string& str);


    /*****************************************************************************************************/

                /*******************************************************************\
                 *                            MISC                                 *
                 *******************************************************************
                 *******************************************************************
                 *                                                                 *
                 *                           OTHERS                                *
                 *                                                                 *
                 *******************************************************************
                 *******************************************************************
                \*******************************************************************/

    ///
    /// \brief prepareData Helper method to encrypt data with symmetric key and convert it in to tranferable data.
    /// \param clientId Extra text in between representing client ID (leave empty if you don't need it)
    /// \param ivec Init vector, if empty, random is generated
    /// \return Base64 format of encrypted data with format: <pre>[LENGTH]:[IV]:[[Client_ID]:]:[Base64 Data]</pre>
    ///
    static std::string prepareData(const std::string& data, const std::string& hexKey, const char* clientId = "", const std::string& ivec = "");

    ///
    /// \brief Calculates expected data size. Assumed IV size = 32
    /// \see prepareData(const char*, const std::string&, const char*)
    ///
    static std::size_t expectedDataSize(std::size_t plainDataSize, std::size_t clientIdSize = 16);

    ///
    /// \brief Helper function to convert string to hexdecimal e.g, khn = 6b686e.
    ///
    static std::string stringToHex(const std::string& raw);

    ///
    /// \brief Helper function to convert hexadecimal input to raw data
    ///
    static std::string hexToString(const std::string& hex);

    ///
    /// \brief Converts vector of RipeByte to raw string
    ///
    static std::string vecToString(const std::vector<RipeByte>& iv);

    ///
    /// \brief ivToVector Converts plain (unsigned char*) IV to std::vector<RipeByte>
    ///
    static std::vector<RipeByte> RipeByteToVec(const RipeByte* iv);

    ///
    /// \brief version Version of Ripe library
    ///
    static std::string version();

private:

    Ripe() {}
    Ripe(const Ripe&) {}
    Ripe& operator=(const Ripe&) { return *this; }
};
#endif /* Ripe_h */
