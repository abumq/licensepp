//
//  issuing-authority.h
//  License++
//
//  Copyright © 2018-present Muflihun Labs
//

#ifndef IssuingAuthority_h
#define IssuingAuthority_h

#include <string>
#include <license++/license.h>

namespace license {

///
/// \brief License issuing authority
///
class IssuingAuthority
{
public:
    IssuingAuthority(const std::string& id, const std::string& name, const std::string& keypair, unsigned int maxValidity, bool active = true);

    IssuingAuthority(const IssuingAuthority&);
    IssuingAuthority& operator=(IssuingAuthority);

    inline std::string id() const
    {
        return m_id;
    }

    inline std::string name() const
    {
        return m_name;
    }

    inline bool active() const
    {
        return m_active;
    }

    inline unsigned int maxValidity() const
    {
        return m_maxValidity;
    }

    License issue(const std::string& licensee,
                  unsigned int validityPeriod,
                  const std::string& masterKey,
                  const std::string& secret = "",
                  const std::string& licenseeSignature = "") const;

    bool validate(const License* license,
                  const std::string& masterKey,
                  bool validateSignature,
                  const std::string& licenseeSignature = "") const;

private:
    std::string m_id;
    std::string m_name;
    std::string m_keypair;
    bool m_active;
    unsigned int m_maxValidity;
};
}

#endif /* IssuingAuthority_h */
