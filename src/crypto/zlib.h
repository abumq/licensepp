//
//  zlib.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef ZLib_h
#define ZLib_h

#include <string>

namespace license {

///
/// \brief ZLib wrappers
///
class ZLib
{
public:

    ///
    /// \brief Deflate
    ///
    static std::string compress(const std::string& data);

    ///
    /// \brief Inflate
    ///
    static std::string decompress(const std::string& data);

    ///
    /// \brief Compress file (Deflate)
    ///
    static bool compressFile(const std::string& gzoutFilename, const std::string& inputFile);

};
}

#endif /* ZLib_h */
