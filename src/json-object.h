//
//  json-object.h
//  License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef JsonObject_h
#define JsonObject_h

#include <iostream>
#include "src/external/json.h"

namespace licensepp {

///
/// \brief Json wrapper to serialize / deserialize json string
///
class JsonObject
{
public:
    using Json = nlohmann::json;
    using Keys = std::vector<const char*>;

    explicit JsonObject();

    JsonObject(const Json& newRoot);
    JsonObject(Json&& newRoot);
    JsonObject(std::string&& jsonStr);

    inline bool isValid() const
    {
        return m_isValid;
    }

    inline std::string lastError() const
    {
        return m_lastError;
    }

    inline void setLastError(const std::string& lastError)
    {
        m_lastError = lastError;
    }

    inline const std::string& jsonStr() const
    {
        return m_jsonStr;
    }

    inline Json root(void) const
    {
        return m_root;
    }

    inline unsigned int getUInt(const std::string& key,
                                unsigned int defaultValue = 0) const
    {
        return get<unsigned int>(key, defaultValue);
    }

    inline unsigned long getULong(const std::string& key,
                                  unsigned long defaultValue = 0L) const
    {
        return get<unsigned long>(key, defaultValue);
    }

    inline std::string getString(const std::string& key,
                                 const std::string& defaultValue = "") const
    {
        return get<std::string>(key, defaultValue);
    }

    inline bool getBool(const std::string& key,
                        bool defaultValue = false) const
    {
        return get<bool>(key, defaultValue);
    }

    inline const char* getCString(const std::string& key,
                                  const char* defaultValue = "") const
    {
        return getString(key, defaultValue).c_str();
    }

    inline bool hasKey(const std::string& key) const
    {
         return m_root.count(key) > 0;
    }

    bool hasKeys(const Keys* keys) const;

    template <typename T>
    T get(const std::string& key, const T& defaultValue) const
    {
        if (!hasKey(key)) {
            return defaultValue;
        }
        try {
            return m_root[key];
        } catch (std::exception& e) {
            std::cerr << "Exception thrown when reading key " << key << std::endl
                      << "Exception: " << e.what() << std::endl;
            return defaultValue;
        }
    }
private:
    std::string m_jsonStr;
    Json m_root;
    bool m_isValid;
    std::string m_lastError;
};
}

#endif /* JsonObject_h */
